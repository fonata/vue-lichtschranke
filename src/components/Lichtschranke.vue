<!-- Diese Datei definiert eine einzige Lichtschranke -->

<template>
  <div class="lichtschranke" v-on:click="sagDeinenNamen">
    <span v-bind:class="{ active: value == 1, lampe: 1 }">
      <!-- das folgende Zeichen ist ein Kreis -->
      &#x25cf;
    </span>
    <span>{{ name }}</span>
  </div>
</template>

<script>
import eventBus from "../eventBus";

export default {
  name: "Lichtschranke",

  data() {
    return {
      name: 1234,
      value: 1234,
    };
  },

  created: function () {
    var lichtschranke = this;
    eventBus.$on("update-input", function (nameUndWert) {
      if (nameUndWert.name === lichtschranke.name) {
        console.log(lichtschranke.name + " fühlt sich angesprochen.");
        lichtschranke.value = nameUndWert.value;
      }
    });
  },

  methods: {
    sagDeinenNamen: function () {
      console.log("Hello, ich bin " + this.name);
    },
  },
};
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
  width: 150px;
  text-align: left;
  padding: 5px;
  border: 1px solid black;
  margin-bottom: -1px;
}
</style>
