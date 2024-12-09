<script lang="ts" setup>
import Flag from './Flag.vue';
import '../styles/FlagSet.css';
</script>

<template>
  <div class="flags" data-tauri-drag-region>
    <h2>FLAGS:</h2>
    <div>
      <Flag :checked="true" title="Unlink from PsInitialSystemProcess" />
      <Flag :checked="false" title="Remove ID from PspCidTable" />
      <Flag :checked="false" title="Remove SystemThread flag" />
    </div>
  </div>
</template>
