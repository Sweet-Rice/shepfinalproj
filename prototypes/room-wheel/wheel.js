/**
 * PFT Sun Run — Classroom Assignment Wheel Prototype
 * Logic & Wheel Controller (Issue #22)
 *
 * Implements standalone classic JavaScript according to the cross-file DOM/math/event contract:
 * - Populates dynamic SVG sectors and room list from local demo room definitions.
 * - Computes clockwise rotation math so the selected sector center aligns beneath the 12 o'clock pointer.
 * - Animates rotation via requestAnimationFrame with smooth quartic ease-out.
 * - Guarantees atomic, single-choice selection (pre-skip, mid-spin reveal, and animation settle on the same room).
 * - Dispatches bubbling CustomEvent('room-selected') on #app exactly once per spin.
 * - Supports keyboard navigation, pre-spin skip checkbox, and system prefers-reduced-motion.
 * - Manages accessible live status updates and focus transitions without test-only exposed APIs.
 */

(function () {
  'use strict';

  // Local demo room dataset (unverified placeholders for prototype demonstration)
  var ROOMS = [
    { id: 'demo-101', label: '101' },
    { id: 'demo-102', label: '102' },
    { id: 'demo-103', label: '103' },
    { id: 'demo-104', label: '104' },
    { id: 'demo-105', label: '105' },
    { id: 'demo-106', label: '106' }
  ];

  // Palette with warm cream / charcoal and sun orange/gold accents matching styles.css
  var SECTOR_COLORS = [
    { fill: '#f59e0b', text: '#18181b', stroke: '#27272a' }, // Sun gold (high-contrast dark text)
    { fill: '#fef3c7', text: '#18181b', stroke: '#27272a' }, // Amber cream
    { fill: '#d97706', text: '#18181b', stroke: '#27272a' }, // Sun orange
    { fill: '#fff8eb', text: '#18181b', stroke: '#27272a' }, // Warm cream
    { fill: '#c2410c', text: '#ffffff', stroke: '#27272a' }, // Sun ember (white text)
    { fill: '#fde68a', text: '#18181b', stroke: '#27272a' }  // Light sun gold
  ];

  var SVG_NS = 'http://www.w3.org/2000/svg';
  var CENTER_X = 300;
  var CENTER_Y = 300;
  var RADIUS = 240;
  var SPIN_DURATION_MS = 2400; // ~2.4 seconds

  // DOM element references
  var appEl = null;
  var wheelRotorEl = null;
  var spinButtonEl = null;
  var skipButtonEl = null;
  var skipAnimationCheckboxEl = null;
  var assignedRoomEl = null;
  var resultDetailsEl = null;
  var statusEl = null;
  var roomListEl = null;

  // Transient state
  var currentRotation = 0;
  var isSpinning = false;
  var animationFrameId = null;
  var spinGeneration = 0;
  var activeChosenIndex = null;
  var activeTargetRotation = 0;

  /**
   * Convert polar coordinates (degrees clockwise from 12 o'clock) to Cartesian SVG coordinates.
   * At 0 deg: (300, 300 - r) -> 12 o'clock.
   * At 90 deg: (300 + r, 300) -> 3 o'clock.
   */
  function polarToCartesian(cx, cy, r, angleDegrees) {
    var rad = (angleDegrees * Math.PI) / 180;
    return {
      x: cx + r * Math.sin(rad),
      y: cy - r * Math.cos(rad)
    };
  }

  /**
   * Build an SVG pie wedge path descriptor for sector i spanning 60 degrees.
   */
  function buildSectorPath(cx, cy, r, startAngle, endAngle) {
    var startPt = polarToCartesian(cx, cy, r, startAngle);
    var endPt = polarToCartesian(cx, cy, r, endAngle);

    // Large-arc is 0 because 60 deg <= 180 deg. Sweep is 1 for clockwise arc.
    return [
      'M', cx, cy,
      'L', startPt.x.toFixed(3), startPt.y.toFixed(3),
      'A', r, r, 0, 0, 1, endPt.x.toFixed(3), endPt.y.toFixed(3),
      'Z'
    ].join(' ');
  }

  /**
   * Populate the SVG rotor group with 6 sector slices and labels.
   */
  function buildWheelSectors() {
    if (!wheelRotorEl) return;
    wheelRotorEl.innerHTML = '';

    var sectorCount = ROOMS.length;
    var sectorSpan = 360 / sectorCount; // 60 degrees

    for (var i = 0; i < sectorCount; i++) {
      var room = ROOMS[i];
      var color = SECTOR_COLORS[i % SECTOR_COLORS.length];
      var startAngle = i * sectorSpan;
      var endAngle = (i + 1) * sectorSpan;
      var centerAngle = (i + 0.5) * sectorSpan;

      var sectorG = document.createElementNS(SVG_NS, 'g');
      sectorG.setAttribute('class', 'wheel-sector');
      sectorG.setAttribute('data-room-id', room.id);
      sectorG.setAttribute('data-sector-index', String(i));

      // Sector wedge
      var wedgePath = document.createElementNS(SVG_NS, 'path');
      wedgePath.setAttribute('d', buildSectorPath(CENTER_X, CENTER_Y, RADIUS, startAngle, endAngle));
      wedgePath.setAttribute('fill', color.fill);
      wedgePath.setAttribute('stroke', color.stroke);
      wedgePath.setAttribute('stroke-width', '2');
      wedgePath.setAttribute('stroke-linejoin', 'round');
      wedgePath.setAttribute('aria-label', 'Room ' + room.label + ' (demo placeholder)');
      sectorG.appendChild(wedgePath);

      // Label group rotated by centerAngle so text is centered and upright under the pointer
      var labelG = document.createElementNS(SVG_NS, 'g');
      labelG.setAttribute('transform', 'rotate(' + centerAngle.toFixed(3) + ' ' + CENTER_X + ' ' + CENTER_Y + ')');

      // "ROOM" header
      var textPrefix = document.createElementNS(SVG_NS, 'text');
      textPrefix.setAttribute('x', String(CENTER_X));
      textPrefix.setAttribute('y', '125');
      textPrefix.setAttribute('text-anchor', 'middle');
      textPrefix.setAttribute('fill', color.text);
      textPrefix.setAttribute('font-size', '12');
      textPrefix.setAttribute('font-weight', '700');
      textPrefix.setAttribute('letter-spacing', '1.5');
      textPrefix.textContent = 'ROOM';
      labelG.appendChild(textPrefix);

      // Room number (large)
      var textNum = document.createElementNS(SVG_NS, 'text');
      textNum.setAttribute('x', String(CENTER_X));
      textNum.setAttribute('y', '158');
      textNum.setAttribute('text-anchor', 'middle');
      textNum.setAttribute('fill', color.text);
      textNum.setAttribute('font-size', '28');
      textNum.setAttribute('font-weight', '800');
      textNum.textContent = room.label;
      labelG.appendChild(textNum);

      // "demo" subscript
      var textSub = document.createElementNS(SVG_NS, 'text');
      textSub.setAttribute('x', String(CENTER_X));
      textSub.setAttribute('y', '180');
      textSub.setAttribute('text-anchor', 'middle');
      textSub.setAttribute('fill', color.text);
      textSub.setAttribute('font-size', '11');
      textSub.setAttribute('font-weight', '600');
      textSub.setAttribute('opacity', '0.85');
      textSub.textContent = 'demo';
      labelG.appendChild(textSub);

      sectorG.appendChild(labelG);
      wheelRotorEl.appendChild(sectorG);
    }
  }

  /**
   * Populate sample room list items in ul#room-list.
   */
  function buildRoomList() {
    if (!roomListEl) return;
    roomListEl.innerHTML = '';

    for (var i = 0; i < ROOMS.length; i++) {
      var room = ROOMS[i];
      var li = document.createElement('li');
      li.id = 'room-item-' + room.id;
      li.className = 'room-list-item';
      li.dataset.roomId = room.id;
      li.dataset.roomIndex = String(i);
      li.textContent = 'Room ' + room.label + ' (demo)';
      roomListEl.appendChild(li);
    }
  }

  /**
   * Update visual highlight on the room list and wheel sectors.
   */
  function updateHighlights(selectedIndex) {
    if (wheelRotorEl) {
      var sectors = wheelRotorEl.querySelectorAll('.wheel-sector');
      for (var s = 0; s < sectors.length; s++) {
        if (s === selectedIndex) {
          sectors[s].classList.add('is-selected');
        } else {
          sectors[s].classList.remove('is-selected');
        }
      }
    }

    if (roomListEl) {
      var items = roomListEl.querySelectorAll('li');
      for (var it = 0; it < items.length; it++) {
        if (it === selectedIndex) {
          items[it].classList.add('is-selected');
          items[it].setAttribute('aria-current', 'true');
        } else {
          items[it].classList.remove('is-selected');
          items[it].removeAttribute('aria-current');
        }
      }
    }
  }

  /**
   * Check whether animation should be skipped (checkbox or OS reduced motion).
   */
  function shouldSkipAnimation() {
    var userSkip = Boolean(skipAnimationCheckboxEl && skipAnimationCheckboxEl.checked);
    var systemReduced = Boolean(
      window.matchMedia && window.matchMedia('(prefers-reduced-motion: reduce)').matches
    );
    return userSkip || systemReduced;
  }

  /**
   * Quartic ease-out function: 1 - (1 - t)^4
   */
  function easeOutQuart(t) {
    var inv = 1 - t;
    return 1 - inv * inv * inv * inv;
  }

  /**
   * Initiate wheel spin.
   */
  function startSpin() {
    // Prevent double-click races during an active spin
    if (isSpinning) {
      return;
    }

    // Step 1: Pre-select one immutable outcome for this spin
    var chosenIndex = Math.floor(Math.random() * ROOMS.length);
    activeChosenIndex = chosenIndex;

    // Step 2: Calculate target angle
    // Sector center angle clockwise from 12 o'clock
    var sectorCenterAngle = (chosenIndex + 0.5) * 60;
    // Final rotation modulo 360 so sector center sits beneath pointer at 12 o'clock
    var targetMod = (360 - sectorCenterAngle) % 360;

    // Calculate clockwise unwrapped rotation
    var fullRevolutions = 5; // Accumulate 5 full turns
    var currentMod = ((currentRotation % 360) + 360) % 360;
    var delta = (targetMod - currentMod) % 360;
    if (delta < 0) {
      delta += 360;
    }
    // Ensure noticeable rotation even if landing on the same sector index
    if (delta < 30) {
      delta += 360;
    }
    var totalSpin = (fullRevolutions * 360) + delta;
    var targetRotation = currentRotation + totalSpin;
    activeTargetRotation = targetRotation;

    // Step 3: Transition to spinning state
    isSpinning = true;
    spinGeneration += 1;
    var currentGeneration = spinGeneration;

    appEl.dataset.state = 'spinning';
    appEl.removeAttribute('data-selected-room');

    spinButtonEl.disabled = true;

    // Clear previous selection highlights during spin
    updateHighlights(-1);

    statusEl.textContent = 'Spinning classroom wheel...';
    assignedRoomEl.textContent = 'Selecting...';
    resultDetailsEl.textContent = 'One room is selected for this spin. Skipping changes only the animation.';

    // Step 4: Check if animation should be bypassed immediately
    if (shouldSkipAnimation()) {
      currentRotation = targetRotation;
      wheelRotorEl.setAttribute('transform', 'rotate(' + currentRotation.toFixed(3) + ' ' + CENTER_X + ' ' + CENTER_Y + ')');
      finishSpin(chosenIndex, true);
      return;
    }

    // Show reveal button for mid-spin skip option
    skipButtonEl.hidden = false;
    skipButtonEl.disabled = false;

    // Step 5: Animate via requestAnimationFrame
    var startAngle = currentRotation;
    var startTime = null;

    function renderFrame(timestamp) {
      // Guard against stale rAF callbacks
      if (spinGeneration !== currentGeneration || !isSpinning) {
        return;
      }

      if (!startTime) {
        startTime = timestamp;
      }

      var elapsed = timestamp - startTime;
      var progress = Math.min(1, elapsed / SPIN_DURATION_MS);
      var eased = easeOutQuart(progress);
      var angle = startAngle + totalSpin * eased;

      wheelRotorEl.setAttribute('transform', 'rotate(' + angle.toFixed(3) + ' ' + CENTER_X + ' ' + CENTER_Y + ')');

      if (progress < 1) {
        animationFrameId = requestAnimationFrame(renderFrame);
      } else {
        currentRotation = targetRotation;
        wheelRotorEl.setAttribute('transform', 'rotate(' + currentRotation.toFixed(3) + ' ' + CENTER_X + ' ' + CENTER_Y + ')');
        finishSpin(chosenIndex, false);
      }
    }

    animationFrameId = requestAnimationFrame(renderFrame);
  }

  /**
   * Mid-spin reveal: snaps the wheel to the pre-chosen sector immediately.
   */
  function revealNow() {
    if (!isSpinning) {
      return;
    }

    if (animationFrameId !== null) {
      cancelAnimationFrame(animationFrameId);
      animationFrameId = null;
    }

    currentRotation = activeTargetRotation;
    wheelRotorEl.setAttribute('transform', 'rotate(' + currentRotation.toFixed(3) + ' ' + CENTER_X + ' ' + CENTER_Y + ')');
    finishSpin(activeChosenIndex, true);
  }

  /**
   * Finalize the spin: update DOM state, accessibility announcements, and dispatch handoff event.
   */
  function finishSpin(chosenIndex, wasSkipped) {
    if (!isSpinning) {
      return;
    }
    isSpinning = false;
    animationFrameId = null;

    var room = ROOMS[chosenIndex];

    // Update main container data attributes
    appEl.dataset.state = 'settled';
    appEl.dataset.selectedRoom = room.id;

    // Update spin button
    spinButtonEl.disabled = false;
    var btnTextSpan = spinButtonEl.querySelector('.btn-text');
    if (btnTextSpan) {
      btnTextSpan.textContent = 'Spin again';
    } else {
      spinButtonEl.textContent = 'Spin again';
    }

    // Hide skip button; return focus to spinButton if skipButton was active
    var skipWasFocused = (document.activeElement === skipButtonEl);
    skipButtonEl.hidden = true;
    skipButtonEl.disabled = true;
    if (skipWasFocused) {
      spinButtonEl.focus();
    }

    // Update result displays
    assignedRoomEl.textContent = 'Room ' + room.label + ' (demo)';
    resultDetailsEl.textContent = 'Placeholder room ' + room.label + ' (' + room.id + '). This is the handoff point for the game; no countdown or Unreal connection runs on this page.';

    // Single polite live region announcement
    if (wasSkipped) {
      statusEl.textContent = 'Selection revealed. Assigned Room ' + room.label + ' (demo placeholder).';
    } else {
      statusEl.textContent = 'Wheel settled. Assigned Room ' + room.label + ' (demo placeholder).';
    }

    // Update active highlight on sector and room list
    updateHighlights(chosenIndex);

    // Dispatch bubbling CustomEvent exactly once per spin
    var handoffEvent = new CustomEvent('room-selected', {
      bubbles: true,
      detail: {
        roomId: room.id,
        label: room.label
      }
    });
    appEl.dispatchEvent(handoffEvent);
  }

  /**
   * Initialize prototype when DOM is ready.
   */
  function init() {
    appEl = document.getElementById('app');
    wheelRotorEl = document.getElementById('wheel-rotor');
    spinButtonEl = document.getElementById('spin-button');
    skipButtonEl = document.getElementById('skip-button');
    skipAnimationCheckboxEl = document.getElementById('skip-animation');
    assignedRoomEl = document.getElementById('assigned-room');
    resultDetailsEl = document.getElementById('result-details');
    statusEl = document.getElementById('status');
    roomListEl = document.getElementById('room-list');

    if (!appEl || !wheelRotorEl || !spinButtonEl || !assignedRoomEl) {
      return;
    }

    // Populate dynamic SVG sectors and room list
    buildWheelSectors();
    buildRoomList();

    // Ensure clean initial state
    appEl.dataset.state = 'idle';
    appEl.removeAttribute('data-selected-room');
    if (skipButtonEl) {
      skipButtonEl.hidden = true;
      skipButtonEl.disabled = true;
    }

    // Attach native event listeners
    spinButtonEl.addEventListener('click', startSpin);

    if (skipButtonEl) {
      skipButtonEl.addEventListener('click', revealNow);
    }
  }

  // Support both deferred execution and direct loading
  if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
  } else {
    init();
  }
})();
