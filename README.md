# Smart Vest ESPRIM : simulation Wokwi et réception MQTT

## Lancer la simulation
1. Ouvrir le projet Wokwi « gilet » (ESP32 DevKit).
2. Vérifier que `wokwi/sketch/sketch.ino` utilise les topics `esprim/smartvest/dupont/...`.
3. Cliquer sur ▶ pour démarrer la simulation.
4. Suivre les messages sur le client web HiveMQ (`hivemq.com/demos/websocket-client`), avec l'abonnement `esprim/smartvest/#`.

## Structure du dépôt
- `wokwi/` : code (`sketch/sketch.ino`), diagramme (`diagram.json`) et bibliothèques (`libraries.txt`)
- `data/` : captures de preuve des tests T2 à T5
- `tests/` : tableau des résultats (`resultats_tests.md`)