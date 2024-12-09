<script lang="ts" setup>
import '../styles/Flag.css';

defineProps({
  checked: {
    type: Boolean,
    required: true,
  },
  title: {
    type: String,
    required: true,
  },
});
</script>

<template>
  <li :class="['flag-item', { success: checked, error: !checked }]">
    <img :src="checked ? '/checkmark.png' : '/failed.png'" />
    {{ title }}
  </li>
</template>
