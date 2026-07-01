const state = {
  scenario: 'Zombie Outbreak',
  scene: 'Safe Zone',
  phase: 'LOBBY',
  players: [
    { id: 'PLAYER_01', state: 'ALIVE', battery: 92 },
    { id: 'PLAYER_02', state: 'INFECTED', battery: 78 },
    { id: 'PLAYER_03', state: 'SAFE', battery: 88 }
  ],
  nodes: [
    { id: 'BASE_01', type: 'Base', role: 'SAFE_ZONE' },
    { id: 'RELAY_01', type: 'Relay', outputs: 8 },
    { id: 'AUDIO_01', type: 'Audio', track: '0103' }
  ],
  log: ['Command Center started in demo mode']
};

const app = document.getElementById('app');
const tabs = document.getElementById('tabs');
const clock = document.getElementById('clock');

function addLog(message) {
  const time = new Date().toLocaleTimeString();
  state.log.unshift(`${time} ${message}`);
  render(currentPage);
}

let currentPage = 'dashboard';

function card(title, content) {
  return `<section class="card"><h3>${title}</h3>${content}</section>`;
}

function renderDashboard() {
  app.innerHTML = `
    <div class="grid">
      ${card('Scenario', `<div class="value">${state.scenario}</div>`)}
      ${card('Scene', `<div class="value">${state.scene}</div>`)}
      ${card('Game Phase', `<div class="value">${state.phase}</div>`)}
      ${card('Players', `<div class="value">${state.players.length}</div>`)}
      ${card('Nodes', `<div class="value">${state.nodes.length}</div>`)}
      ${card('Event Log', `<div class="log">${state.log.slice(0, 8).join('<br>')}</div>`)}
    </div>
  `;
}

function renderGame() {
  app.innerHTML = `
    ${card('Game Control', `
      <div class="row">
        <button onclick="setPhase('RUNNING')">Start</button>
        <button onclick="setPhase('PAUSED')">Pause</button>
        <button onclick="setPhase('RUNNING')">Resume</button>
        <button onclick="setPhase('ENDED')">End</button>
        <button class="danger" onclick="emergencyStop()">Emergency Stop</button>
      </div>
    `)}
  `;
}

function renderPlayers() {
  app.innerHTML = `<div class="grid">${state.players.map(player => card(player.id, `
    <p>Status: <strong>${player.state}</strong></p>
    <p>Battery: ${player.battery}%</p>
    <div class="row">
      <button onclick="setPlayer('${player.id}','ALIVE')">Heal</button>
      <button onclick="setPlayer('${player.id}','INFECTED')">Infect</button>
      <button onclick="setPlayer('${player.id}','ELIMINATED')">Eliminate</button>
    </div>
  `)).join('')}</div>`;
}

function renderNodes() {
  app.innerHTML = `<div class="grid">${state.nodes.map(node => card(node.id, `
    <p>Type: ${node.type}</p>
    <p>${node.role ? `Role: ${node.role}` : ''}${node.outputs ? `Outputs: ${node.outputs}` : ''}${node.track ? `Track: ${node.track}` : ''}</p>
    <button onclick="addLog('Ping sent to ${node.id}')">Ping</button>
  `)).join('')}</div>`;
}

function renderLighting() {
  app.innerHTML = card('NeoPixel Control', `
    <p>Status ring and decor strip test controls.</p>
    <div class="row">
      <button onclick="addLog('Status ring green')">Status Green</button>
      <button onclick="addLog('Decor strip pulse')">Decor Pulse</button>
      <button onclick="addLog('Lighting test pattern')">Test Pattern</button>
    </div>
  `);
}

function renderAudio() {
  app.innerHTML = card('Audio Manager', `
    <p>DFPlayer and Audio Node controls.</p>
    <div class="row">
      <button onclick="addLog('Play boot sound')">Boot</button>
      <button onclick="addLog('Play activation sound')">Activate</button>
      <button onclick="addLog('Stop audio')">Stop</button>
    </div>
  `);
}

function renderRelay() {
  app.innerHTML = card('Relay Node Control', `
    <div class="row">
      <button onclick="addLog('Relay 1 ON')">Relay 1 ON</button>
      <button onclick="addLog('Relay 1 OFF')">Relay 1 OFF</button>
      <button onclick="addLog('Relay 1 pulse')">Pulse</button>
      <button class="danger" onclick="addLog('All relays OFF')">All OFF</button>
    </div>
  `);
}

function renderScenes() {
  const scenes = ['Safe Zone', 'Scanner', 'Generator', 'Objective', 'Extraction', 'Zombie Nest'];
  app.innerHTML = `<div class="grid">${scenes.map(scene => card(scene, `<button onclick="loadScene('${scene}')">Load Scene</button>`)).join('')}</div>`;
}

function renderDiagnostics() {
  app.innerHTML = card('Diagnostics', `
    <p>Mode: Demo</p>
    <p>ESP-NOW: Pending firmware connection</p>
    <p>WebSocket: Not connected</p>
    <div class="log">${state.log.join('<br>')}</div>
  `);
}

function render(page) {
  currentPage = page;
  if (page === 'dashboard') renderDashboard();
  else if (page === 'game') renderGame();
  else if (page === 'players') renderPlayers();
  else if (page === 'nodes') renderNodes();
  else if (page === 'lighting') renderLighting();
  else if (page === 'audio') renderAudio();
  else if (page === 'relay') renderRelay();
  else if (page === 'scenes') renderScenes();
  else renderDiagnostics();
}

function setPhase(phase) {
  state.phase = phase;
  addLog(`Game phase set to ${phase}`);
}

function emergencyStop() {
  state.phase = 'EMERGENCY';
  addLog('EMERGENCY STOP triggered');
}

function setPlayer(id, newState) {
  const player = state.players.find(p => p.id === id);
  if (player) {
    player.state = newState;
    addLog(`${id} set to ${newState}`);
  }
}

function loadScene(scene) {
  state.scene = scene;
  addLog(`Scene loaded: ${scene}`);
}

document.getElementById('emergencyStop').addEventListener('click', emergencyStop);

tabs.addEventListener('click', event => {
  if (event.target.tagName !== 'BUTTON') return;
  document.querySelectorAll('.tabs button').forEach(button => button.classList.remove('active'));
  event.target.classList.add('active');
  render(event.target.dataset.page);
});

setInterval(() => {
  clock.textContent = new Date().toLocaleTimeString();
}, 1000);

render('dashboard');
