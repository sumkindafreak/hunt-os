#ifndef HUNT_WEB_ASSETS_H
#define HUNT_WEB_ASSETS_H

#include <Arduino.h>

// =====================================================
// HUNT Command Center starter PROGMEM assets
//
// These are intentionally compact starter assets.
// Later, tools/build_webui.py will generate this file
// automatically from web/command-center/.
// =====================================================

const char HUNT_WEB_INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>HUNT Command Center</title>
<link rel="stylesheet" href="/style.css">
</head>
<body>
<header><h1>HUNT OS</h1><button onclick="cmd('/api/emergency')">EMERGENCY STOP</button></header>
<main>
<section><h2>Dashboard</h2><pre id="system">Loading...</pre></section>
<section><h2>Game</h2><button onclick="cmd('/api/game/start')">Start</button><button onclick="cmd('/api/game/pause')">Pause</button><button onclick="cmd('/api/game/end')">End</button></section>
<section><h2>Base</h2><button onclick="cmd('/api/scene/safe')">Safe Zone</button><button onclick="cmd('/api/scene/scanner')">Scanner</button><button onclick="cmd('/api/scene/objective')">Objective</button><button onclick="cmd('/api/scene/extraction')">Extraction</button></section>
<section><h2>Lighting</h2><button onclick="cmd('/api/light/status')">Status Flash</button><button onclick="cmd('/api/light/decor')">Decor Flash</button></section>
<section><h2>Audio</h2><button onclick="cmd('/api/audio/boot')">Boot Sound</button><button onclick="cmd('/api/audio/activate')">Activate Sound</button><button onclick="cmd('/api/audio/stop')">Stop</button></section>
<section><h2>Log</h2><pre id="log"></pre></section>
</main>
<script src="/app.js"></script>
</body>
</html>
)rawliteral";

const char HUNT_WEB_STYLE_CSS[] PROGMEM = R"rawliteral(
body{margin:0;background:#05070a;color:#e8fff7;font-family:Arial,Helvetica,sans-serif}header{display:flex;justify-content:space-between;align-items:center;padding:14px;background:#07110e;border-bottom:1px solid #00ff9c44}h1{margin:0;color:#00ff9c;letter-spacing:2px}main{padding:12px;display:grid;gap:12px}section{background:#101820;border:1px solid #00ff9c33;border-radius:14px;padding:12px}button{margin:4px;padding:10px;border-radius:10px;border:1px solid #00ff9c66;background:#162433;color:#e8fff7;font-weight:bold}header button{border-color:#ff3455;background:#661424}pre{white-space:pre-wrap;color:#9affd3}
)rawliteral";

const char HUNT_WEB_APP_JS[] PROGMEM = R"rawliteral(
const log=document.getElementById('log');
function add(m){log.textContent=new Date().toLocaleTimeString()+" "+m+"\n"+log.textContent;}
async function refresh(){try{const r=await fetch('/api/system');document.getElementById('system').textContent=JSON.stringify(await r.json(),null,2);}catch(e){add('offline');}}
async function cmd(url){try{await fetch(url,{method:'POST'});add(url);}catch(e){add('failed '+url);}refresh();}
setInterval(refresh,2000);refresh();
)rawliteral";

#endif
