#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <immintrin.h>

uint64_t sip_hash(const uint64_t keys[], const uint8_t* bytes, const uint64_t
size) {
  __v2di key = {keys[0], keys[1]};
  __v2di init0 = key ^ (__v2di){0x646f72616e646f6dull, 0x736f6d6570736575ull};
  __v2di init1 = key ^ (__v2di){0x7465646279746573ull, 0x6c7967656e657261ull};
  __v2di v20 = _mm_unpacklo_epi64(init0, init1);
  __v2di v31 = _mm_unpackhi_epi64(init0, init1);

  size_t offset = 0;
  void *buffer = _mm_malloc(size, 16);
  memcpy(buffer, bytes, size);

  for (; offset < (size & ~7); offset += 8) {
    __v2di packet = _mm_cvtsi64_si128(*(uint64_t*)(buffer+offset));
    _update_sip_hash(packet);
  }

  return v20[0] + v31[1];
}


