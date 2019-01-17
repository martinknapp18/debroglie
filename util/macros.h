#define WRITE_IO(PORT, ON_BITS, OFF_BITS)                                      \
  PORT->ODR = ((PORT->ODR & ~(OFF_BITS)) | (ON_BITS))
#define BITS_NONE (0)

#define ARRAYSIZE(a)                                                           \
  ((sizeof(a) / sizeof(*(a))) /                                                \
   static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

#define STOP  while(1);