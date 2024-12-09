<script setup lang="ts">
import FlagSet from './components/FlagSet.vue';
import Header from './components/Header.vue';
</script>

<template>
  <div class="container">
    <Header />
    <FlagSet />
  </div>
</template>

<style scoped>
@import url('https://fonts.googleapis.com/css2?family=Inter:ital,opsz,wght@0,14..32,100..900;1,14..32,100..900&display=swap');

.container {
  background-color: #1B1B1F;
  color: white;
  font-family: Inter, sans-serif;
  border: 1px #3F3F3F solid;
  border-radius: 10px;
  padding: 20px;
  max-width: 400px;
  height: 560px;
}
</style>
