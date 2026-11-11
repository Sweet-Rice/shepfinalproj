"""Check relative inline Markdown links and ATX heading anchors in tracked docs."""

import re
import subprocess
import sys
from collections import Counter
from functools import cache
from pathlib import Path
from urllib.parse import unquote, urlsplit


INLINE_LINK = re.compile(r"\]\((?:<([^>]+)>|([^\s()]+))(?:\s+[\"'][^\"']*[\"'])?\)")
HEADING = re.compile(r"^ {0,3}#{1,6}\s+(.+?)\s*#*\s*$")
FENCE = re.compile(r"^ {0,3}(`{3,}|~{3,})")


@cache
def markdown_lines(path):
    fence = ""
    lines = []
    for number, line in enumerate(path.read_text(encoding="utf-8-sig").splitlines(), 1):
        match = FENCE.match(line)
        if match:
            marker = match[1]
            if not fence:
                fence = marker
            elif marker.startswith(fence) and not line[match.end():].strip():
                fence = ""
            continue
        if not fence:
            lines.append((number, line))
    return lines


@cache
def anchors(path):
    used = Counter()
    result = set()
    for _, line in markdown_lines(path):
        match = HEADING.match(line)
        if match:
            text = match[1].lower()
            slug = re.sub(r"[^\w -]", "", text).replace(" ", "-")
            suffix = used[slug]
            candidate = slug if not suffix else f"{slug}-{suffix}"
            while candidate in result:
                suffix += 1
                candidate = f"{slug}-{suffix}"
            used[slug] = suffix + 1
            result.add(candidate)
    return result


def main():
    root = Path(subprocess.check_output(["git", "rev-parse", "--show-toplevel"], text=True).strip())
    tracked = subprocess.check_output(["git", "ls-files", "-z"], cwd=root).decode("utf-8").split("\0")
    errors = []
    files = [root / path for path in tracked if path.lower().endswith(".md")]
    for path in files:
        for number, line in markdown_lines(path):
            line = re.sub(r"`[^`]*`", "", line)
            for match in INLINE_LINK.finditer(line):
                link = match[1] or match[2]
                parts = urlsplit(link)
                if parts.scheme or parts.netloc or parts.path.startswith("/"):
                    continue
                target = (path.parent / unquote(parts.path)).resolve() if parts.path else path
                if not target.is_relative_to(root):
                    errors.append(f"{path.relative_to(root)}:{number}: link leaves repository {link}")
                elif not target.exists():
                    errors.append(f"{path.relative_to(root)}:{number}: missing target {link}")
                elif parts.fragment and target.suffix.lower() == ".md":
                    if unquote(parts.fragment) not in anchors(target):
                        errors.append(f"{path.relative_to(root)}:{number}: missing heading {link}")
    print("\n".join(errors) if errors else f"Checked relative links in {len(files)} Markdown files.")
    return bool(errors)


if __name__ == "__main__":
    sys.exit(main())
