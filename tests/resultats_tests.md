# Résultats des tests : Smart Vest

Date : 07/11/2026
Broker : broker.hivemq.com (MQTT port 1883 pour l'ESP32, WebSocket port 8000 ou 8884 pour le client web)
Simulateur : Wokwi (projet « gilet », ESP32 DevKit)

| Test | Heure | Action | Résultat attendu | Résultat observé | Statut | Preuve |
|---|---|---|---|---|---|---|
| T1 | 15:21 | Démarrage | Wi-Fi et MQTT connectés | Wi-Fi connecté (IP 10.10.0.2), MQTT OK | Validé | moniteur série |
| T2 | 15:22 | Potentiomètre au milieu, DHT22 à 37 °C | bpm ≈ 120, alert:false | <à compléter> | <à confirmer> | data/moniteur_T2_normal.png |
| T3 | 15:22 | Potentiomètre à fond | bpm ≥ 140, pouls_eleve | bpm 179.2, ALERTE : pouls_eleve | Validé | data/moniteur_T3_pouls.png |
| T4 | 15:23 | DHT22 à 40 °C | temp > 39,5, temperature_anormale | temp 40.00, ALERTE : temperature_anormale | Validé | data/moniteur_T4_temperature.png |
| T5 | 15:24 | Réception côté cloud (client web HiveMQ) | Messages telemetry et alert reçus | telemetry et alert reçus sur esprim/smartvest/dupont/ | Validé | data/client_web_T5.png |

## Détails

- Nombre de messages reçus (T5) : <à compléter> sur <durée> secondes
- Fréquence observée des messages telemetry : <à compléter> (attendu : 1 toutes les 2 s)
- Remarque : MQTT Explorer n'a pas reçu les messages. Le client web HiveMQ a été utilisé comme preuve de réception.
# Smart Vest ESPRIM : simulation Wokwi et réception MQTT

## Lancer la simulation
1. Ouvrir le projet Wokwi « gilet » (ESP32 DevKit).
2. Vérifier que `sketch/sketch.ino` contient les topics `esprim/smartvest/dupont/...`.
3. Cliquer sur ▶ pour démarrer.

## Dossiers
- `wokwi/` : code, diagramme et bibliothèques
- `data/` : captures de preuve des tests
- `tests/` : tableau des résultats T1 à T5