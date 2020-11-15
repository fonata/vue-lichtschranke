// Diese Datei existiert, damit wir den Lichtschranken Events schicken können
import Vue from 'vue'

const port = 5000;

var data = {
    socket: new WebSocket("ws://127.0.0.1:" + port)
}

let eventBus = new Vue({
    data: function () {
        return data;
    },
    created: function () {
        this.socket.onopen = function () {
            // Die Verbindung wurde erfolgreich hergestellt.
            // Den Server fragen, was es für Lichtschranken gibt.
            this.send("get-inputs");
        };
        this.socket.onmessage = function (event) {
            try {
                let data = JSON.parse(event.data);
                console.log('The server said: ' + JSON.stringify(data, null, 4));

                // Die Nachricht vom Server innerhalb der Vue-Anwendung weiterverteilen.
                eventBus.$emit(data.event, data.payload)
            } catch (e) {
                console.log('[ERROR] The server sent invalid JSON: ' + event.data);
            }
        }
    }
});

export default eventBus;
