<!-- Diese Datei definiert eine einzige Lichtschranke -->

<template>
  <div class="lichtschranke" v-on:click="sagDeinenNamen">
    <span v-bind:class="{ active: value == '1', lampe: 1 }">
      <!-- das folgende Zeichen ist ein Kreis -->
      &#x25cf;
    </span>
    <span>{{ name }}</span>
  </div>
</template>

<script>
import events from '../events';

export default {
  name: 'HelloWorld',
  props: {
    name: String,
    value: String
  },
  created: function () {
    events.$on('update-lichtschranke', function (nameUndWert) {
      if (nameUndWert.name == this.name) {
        this.value = nameUndWert.value;
      }
    });       
  },
  methods: {
    sagDeinenNamen: function () {
       console.log('Hello, ich bin ' + this.name);       
    }
  }
}
</script>

<style scoped>
.lampe {
  color: red;
  display: inline-block;
  margin-right: 10px;
}
.lampe.active {
  color: green;
}
.lichtschranke {
  /* display: inline-block; */
  width: 150px;
  text-align: left;
  padding: 5px;
  border: 1px solid black;
}
</style>
