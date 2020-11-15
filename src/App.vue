<template>
  <div id="app">
    <img alt="Vue logo" src="./assets/logo.png">
    <fieldset>
      <legend>Lichtschranken</legend>
      <div id="lichtschranken">
        Liste wird geladen...
      </div>
    </fieldset><br>
    <button v-on:click="sendToServer">Nachricht an Server schicken</button>
  </div>
</template>

<script>
import Vue from 'vue'
import Lichtschranke from './components/Lichtschranke.vue'
import eventBus from './eventBus';

export default {
  name: 'App',
  created: function () {
    eventBus.$on('update-lichtschranke', function (nameUndWert) {
      console.log('Ok, ' + nameUndWert.value + ' ist der neue Wert von ' + nameUndWert.name);
    });

    eventBus.$on('set-inputs', function (inputs) {
      let lichtschranken = document.getElementById('lichtschranken');
      lichtschranken.textContent = ''; // Den Text "Bitte warten..." brauchen wir nicht mehr.

      for (let i = 0; i < inputs.length; i++) {
        var ComponentClass = Vue.extend(Lichtschranke);
        var instance = new ComponentClass({
          data() {
            return {
              name: inputs[i].name,
              value: inputs[i].value
            }
          }
        });
        instance.$mount();
        lichtschranken.appendChild(instance.$el);
      }
    });
  },
  methods: {
    sendToServer: function () {
      eventBus.socket.send('wichtige-botschaft-an-den-server')
    }
  }
}


</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}

fieldset {
  display: inline-block;
  box-shadow: 1px 4px 4px #4444dd;
}
</style>
