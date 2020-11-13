<template>
  <div id="app">
    <img alt="Vue logo" src="./assets/logo.png">
    <Lichtschranke name="LS-1" value="1" />
    <Lichtschranke name="LS-2" value="0" />
    <Lichtschranke name="LS-3" value="1" />
    <Lichtschranke name="LS-4" value="1" />
    <Lichtschranke name="LS-5" value="1" />
    <button  v-on:click="machHalt">Sage LS-3, dass sie aus ist</button>
  </div>
</template>

<script>
import Lichtschranke from './components/Lichtschranke.vue'
import events from 'events';
export default {
  name: 'App',
  components: {
    Lichtschranke
  },
  created: function () {
    this.$on('update-lichtschranke', function (nameUndWert) {
      console.log('Ok, ' + nameUndWert.value + ' ist der neue Wert von ' + nameUndWert.name);
    });
  },
  methods: {
    machHalt: function() {
console.log(events);
      events.$emit('update-lichtschranke', {name: 'LS-3', value: '0'})
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
</style>
