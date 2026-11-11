/* Room wheel prototype — DOM/animation layer. Selection logic lives in wheel-logic.js. */
(function () {
  "use strict";
  var L = window.RoomWheelLogic;

  // Placeholder rooms. NOT verified PFT room numbers or routes.
  var ROOMS = [
    { id: "1100", eligible: true },
    { id: "1200", eligible: true },
    { id: "1202", eligible: false },
    { id: "1263", eligible: true },
    { id: "2150", eligible: true },
    { id: "2348", eligible: true },
    { id: "3107", eligible: true },
    { id: "3142", eligible: true },
  ];
  var SPAWN_DEFAULT = "commons-door"; // Commons exterior door: not a classroom
  var SEG_COLORS = ["var(--seg-a)", "var(--seg-b)", "var(--seg-c)", "var(--seg-d)"];
  var SPIN_MS = 4200;
  var MIN_TURNS = 5;

  // ?seed=123 makes the sequence of picks reproducible (useful for QA).
  var seedParam = new URLSearchParams(location.search).get("seed");
  var rng = seedParam !== null ? L.seededRng(Number(seedParam) || 0) : Math.random;

  var $ = function (id) { return document.getElementById(id); };
  var wheel = $("wheel"), wrap = $("wheelWrap"), pointer = $("pointer");
  var spinBtn = $("spinBtn"), skipNowBtn = $("skipNowBtn"), skipAnim = $("skipAnim");
  var resultEl = $("result"), roomList = $("roomList"), spawnSel = $("spawnSel");
  var countdownIn = $("countdown"), countdownVal = $("countdownVal");
  var logEl = $("log"), announcer = $("announcer");

  var state = {
    phase: "idle", // idle | spinning | countdown | sunActive
    rotation: 0,
    eligible: [],
    pending: null, // { index, endRotation, via }
    timers: [],
    raf: 0,
  };

  // Respect reduced-motion by default; the user can still toggle it.
  var reduceMotion = window.matchMedia && matchMedia("(prefers-reduced-motion: reduce)").matches;
  skipAnim.checked = reduceMotion;

  // ---------- Settings panel ----------
  ROOMS.forEach(function (room) {
    var label = document.createElement("label");
    label.className = "check";
    var cb = document.createElement("input");
    cb.type = "checkbox";
    cb.checked = room.eligible;
    cb.addEventListener("change", function () {
      room.eligible = cb.checked;
      resetRound();
    });
    label.appendChild(cb);
    label.appendChild(document.createTextNode(" " + room.id));
    roomList.appendChild(label);
  });

  [{ v: SPAWN_DEFAULT, t: "Commons exterior door" }]
    .concat(ROOMS.map(function (r) { return { v: r.id, t: "Inside room " + r.id }; }))
    .forEach(function (o) {
      var opt = document.createElement("option");
      opt.value = o.v;
      opt.textContent = o.t;
      spawnSel.appendChild(opt);
    });
  spawnSel.addEventListener("change", resetRound);

  countdownIn.addEventListener("input", function () {
    countdownVal.textContent = countdownIn.value;
  });

  // ---------- Wheel drawing ----------
  function polar(r, deg) {
    var a = (deg * Math.PI) / 180;
    return [r * Math.sin(a), -r * Math.cos(a)]; // 0deg = top, clockwise
  }

  function drawWheel() {
    var n = state.eligible.length;
    var svgNS = "http://www.w3.org/2000/svg";
    wheel.innerHTML = "";
    var R = 96;

    var rim = document.createElementNS(svgNS, "circle");
    rim.setAttribute("r", 99);
    rim.setAttribute("fill", "#3a1b2e");
    rim.setAttribute("stroke", "#fff2c8");
    rim.setAttribute("stroke-width", "2");
    wheel.appendChild(rim);

    if (!n) return;
    var size = 360 / n;
    state.eligible.forEach(function (room, i) {
      var color = SEG_COLORS[i % SEG_COLORS.length];
      // Avoid two adjacent segments sharing a colour when wrapping around.
      if (n > 1 && i === n - 1 && i % SEG_COLORS.length === 0) color = SEG_COLORS[1];
      var shape;
      if (n === 1) {
        shape = document.createElementNS(svgNS, "circle");
        shape.setAttribute("r", R);
      } else {
        var p0 = polar(R, i * size), p1 = polar(R, (i + 1) * size);
        shape = document.createElementNS(svgNS, "path");
        shape.setAttribute("d",
          "M0 0 L" + p0[0] + " " + p0[1] +
          " A" + R + " " + R + " 0 " + (size > 180 ? 1 : 0) + " 1 " + p1[0] + " " + p1[1] + " Z");
      }
      shape.setAttribute("fill", color);
      shape.setAttribute("stroke", "#2a0f1a");
      shape.setAttribute("stroke-width", "0.8");
      wheel.appendChild(shape);

      var mid = (i + 0.5) * size;
      var text = document.createElementNS(svgNS, "text");
      text.setAttribute("class", "seg-label");
      text.setAttribute("text-anchor", "middle");
      text.setAttribute("dominant-baseline", "middle");
      text.setAttribute("transform", "rotate(" + mid + ") translate(0 -66) rotate(-90)");
      text.textContent = room.id;
      wheel.appendChild(text);
    });

    var hub = document.createElementNS(svgNS, "circle");
    hub.setAttribute("r", 14);
    hub.setAttribute("class", "hub");
    wheel.appendChild(hub);
  }

  function setRotation(deg, animate) {
    wheel.style.transition = animate
      ? "transform " + SPIN_MS + "ms cubic-bezier(0.12, 0.7, 0.08, 1)"
      : "none";
    wheel.style.transform = "rotate(" + deg + "deg)";
    state.rotation = deg;
  }

  // Pointer "ticks" whenever a segment border passes it (purely cosmetic).
  function watchTicks() {
    var last = -1;
    function frame() {
      var m = getComputedStyle(wheel).transform;
      var match = /matrix\(([^,]+),\s*([^,]+)/.exec(m);
      if (match) {
        var deg = (Math.atan2(parseFloat(match[2]), parseFloat(match[1])) * 180) / Math.PI;
        var seg = L.segmentAtRotation(deg, state.eligible.length);
        if (seg !== last && last !== -1) {
          pointer.classList.remove("tick");
          void pointer.offsetWidth;
          pointer.classList.add("tick");
        }
        last = seg;
      }
      if (state.phase === "spinning") state.raf = requestAnimationFrame(frame);
    }
    state.raf = requestAnimationFrame(frame);
  }

  // ---------- Round flow ----------
  function clearTimers() {
    state.timers.forEach(clearTimeout);
    state.timers = [];
    cancelAnimationFrame(state.raf);
  }

  function resetRound() {
    clearTimers();
    state.phase = "idle";
    state.pending = null;
    var spawn = spawnSel.value;
    state.eligible = L.filterEligible(ROOMS, spawn);
    drawWheel();
    skipNowBtn.hidden = true;
    wrap.classList.remove("spinning");
    countdownIn.disabled = false;
    if (!state.eligible.length) {
      spinBtn.disabled = true;
      spinBtn.textContent = "Spin";
      resultEl.innerHTML = '<p class="error">Setup problem: no eligible rooms. Round stays inactive.</p>';
      return;
    }
    spinBtn.disabled = false;
    spinBtn.textContent = "Spin";
    resultEl.innerHTML = '<p class="small">Spin to get your target classroom.</p>';
  }

  /**
   * The single entry point for every input (mouse, keyboard, skip checkbox).
   * 1) choose the room, 2) animate (or not) to it, 3) finishSpin().
   */
  function spin(via) {
    if (state.phase === "spinning") return;
    if (state.phase !== "idle") resetRound(); // "Spin again" = new round
    if (!state.eligible.length) return;

    var n = state.eligible.length;
    var index = L.pickIndex(state.eligible, rng);
    var jitter = rng() * 2 - 1;
    var end = L.targetRotation(state.rotation, index, n, MIN_TURNS, jitter);
    state.pending = { index: index, endRotation: end, via: via };
    state.phase = "spinning";
    spinBtn.disabled = true;
    countdownIn.disabled = true;

    if (skipAnim.checked) {
      finishSpin(true);
      return;
    }
    skipNowBtn.hidden = false;
    wrap.classList.add("spinning");
    announce("Spinning…");
    setRotation(end, true);
    watchTicks();
    // transitionend can be missed (tab hidden etc.); a timer is the source of truth.
    state.timers.push(setTimeout(function () { finishSpin(false); }, SPIN_MS + 50));
  }

  /** Jump straight to the already-chosen result. */
  function skipNow() {
    if (state.phase === "spinning") finishSpin(true);
  }

  function finishSpin(skipped) {
    if (state.phase !== "spinning" || !state.pending) return;
    clearTimers();
    var p = state.pending;
    setRotation(p.endRotation, false);
    skipNowBtn.hidden = true;
    wrap.classList.remove("spinning");

    var n = state.eligible.length;
    var landed = L.segmentAtRotation(p.endRotation, n);
    if (landed !== p.index) {
      // Should be impossible; the logic tests cover it. Fail loudly if not.
      console.error("Wheel landed on", landed, "but result was", p.index);
    }
    var room = state.eligible[p.index];
    var seconds = L.clamp(Number(countdownIn.value) || 25, 20, 30);

    resultEl.classList.remove("reveal");
    void resultEl.offsetWidth;
    resultEl.classList.add("reveal");
    resultEl.innerHTML =
      '<p class="objective">Find Room <span class="room" id="resultRoom">' + room.id +
      "</span> before the sun kills you.</p>" +
      '<div class="countdown"><span class="count-num" id="countNum">' + seconds + '</span>' +
      '<div class="bar"><i id="countBar"></i></div></div>' +
      '<p class="small" id="countMsg">Sun rises when the countdown hits zero.</p>';
    announce("Target: Room " + room.id + ". " + seconds + " seconds until the sun rises.");

    var detail = {
      roomId: room.id,
      segmentIndex: p.index,
      landedIndex: landed,
      countdownSeconds: seconds,
      via: p.via,
      skipped: skipped,
    };
    emit("roomwheel:selected", detail);
    startCountdown(seconds);
    spinBtn.disabled = false;
    spinBtn.textContent = "Spin again";
  }

  function startCountdown(seconds) {
    state.phase = "countdown";
    emit("roomwheel:countdownStarted", { countdownSeconds: seconds });
    var start = performance.now();
    var num = $("countNum"), bar = $("countBar"), msg = $("countMsg");
    function step() {
      if (state.phase !== "countdown") return;
      var left = Math.max(0, seconds - (performance.now() - start) / 1000);
      num.textContent = Math.ceil(left);
      bar.style.transform = "scaleX(" + left / seconds + ")";
      if (left <= 0) {
        state.phase = "sunActive";
        msg.innerHTML = '<span class="sun-up">☀️ The sun is up — exposure damage is now active!</span>';
        announce("The sun is up.");
        emit("roomwheel:sunActive", {});
        return;
      }
      state.timers.push(setTimeout(step, 100));
    }
    step();
  }

  // ---------- Events / log ----------
  function emit(name, detail) {
    window.dispatchEvent(new CustomEvent(name, { detail: detail }));
    var li = document.createElement("li");
    li.textContent = name.replace("roomwheel:", "") + " " + JSON.stringify(detail);
    logEl.prepend(li);
  }

  function announce(text) {
    announcer.textContent = text;
  }

  // ---------- Input ----------
  // Mouse click or keyboard activation of the focused button (detail === 0).
  spinBtn.addEventListener("click", function (e) {
    spin(e.detail === 0 ? "keyboard" : "pointer");
  });
  skipNowBtn.addEventListener("click", skipNow);
  wheel.addEventListener("click", function () { spin("pointer"); });

  document.addEventListener("keydown", function (e) {
    var tag = e.target.tagName;
    var typing = tag === "INPUT" || tag === "SELECT" || tag === "TEXTAREA";
    var onButton = tag === "BUTTON"; // buttons already handle Space/Enter natively
    if (e.key === "Escape") {
      skipNow();
    } else if ((e.key === " " || e.key === "Enter") && !typing && !onButton) {
      e.preventDefault();
      spin("keyboard");
    } else if ((e.key === "r" || e.key === "R") && !typing && !e.ctrlKey && !e.metaKey) {
      resetRound();
      spinBtn.focus();
    }
  });

  resetRound();
  if (seedParam !== null) emit("roomwheel:seed", { seed: seedParam });
})();
