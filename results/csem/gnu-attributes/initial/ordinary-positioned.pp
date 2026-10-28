# 184 "/lib/tcc/include/tccdefs.h"
    struct
# 184 "/lib/tcc/include/tccdefs.h"
           __uint128__
# 184 "/lib/tcc/include/tccdefs.h"
                       {
# 184 "/lib/tcc/include/tccdefs.h"
                         char
# 184 "/lib/tcc/include/tccdefs.h"
                              x
# 184 "/lib/tcc/include/tccdefs.h"
                               [
# 184 "/lib/tcc/include/tccdefs.h"
                                16
# 184 "/lib/tcc/include/tccdefs.h"
                                  ]
# 184 "/lib/tcc/include/tccdefs.h"
                                   ;
# 184 "/lib/tcc/include/tccdefs.h"
                                     }
# 184 "/lib/tcc/include/tccdefs.h"
                                       __attribute
# 184 "/lib/tcc/include/tccdefs.h"
                                                  (
# 184 "/lib/tcc/include/tccdefs.h"
                                                   (
# 184 "/lib/tcc/include/tccdefs.h"
                                                    __aligned__
# 184 "/lib/tcc/include/tccdefs.h"
                                                               (
# 184 "/lib/tcc/include/tccdefs.h"
                                                                16
# 184 "/lib/tcc/include/tccdefs.h"
                                                                  )
# 184 "/lib/tcc/include/tccdefs.h"
                                                                   )
# 184 "/lib/tcc/include/tccdefs.h"
                                                                    )
# 184 "/lib/tcc/include/tccdefs.h"
                                                                     ;
# 240 "/lib/tcc/include/tccdefs.h"
    typedef
# 240 "/lib/tcc/include/tccdefs.h"
            char
# 240 "/lib/tcc/include/tccdefs.h"
                 *
# 240 "/lib/tcc/include/tccdefs.h"
                  __builtin_va_list
# 240 "/lib/tcc/include/tccdefs.h"
                                   ;
# 279 "/lib/tcc/include/tccdefs.h"
           void
# 279 "/lib/tcc/include/tccdefs.h"
               *
# 279 "/lib/tcc/include/tccdefs.h"
    __builtin_memcpy
# 279 "/lib/tcc/include/tccdefs.h"
                          (
# 279 "/lib/tcc/include/tccdefs.h"
                           void
# 279 "/lib/tcc/include/tccdefs.h"
                                *
# 279 "/lib/tcc/include/tccdefs.h"
                                 ,
# 279 "/lib/tcc/include/tccdefs.h"
                                   const
# 279 "/lib/tcc/include/tccdefs.h"
                                         void
# 279 "/lib/tcc/include/tccdefs.h"
                                             *
# 279 "/lib/tcc/include/tccdefs.h"
                                              ,
# 279 "/lib/tcc/include/tccdefs.h"
                                                unsigned
# 279 "/lib/tcc/include/tccdefs.h"
                                                long
# 279 "/lib/tcc/include/tccdefs.h"
                                                             )
# 279 "/lib/tcc/include/tccdefs.h"
    __asm__
# 279 "/lib/tcc/include/tccdefs.h"
    (
# 279 "/lib/tcc/include/tccdefs.h"
    "memcpy"
# 279 "/lib/tcc/include/tccdefs.h"
    )
# 279 "/lib/tcc/include/tccdefs.h"
    ;
# 280 "/lib/tcc/include/tccdefs.h"
           void
# 280 "/lib/tcc/include/tccdefs.h"
               *
# 280 "/lib/tcc/include/tccdefs.h"
    __builtin_memmove
# 280 "/lib/tcc/include/tccdefs.h"
                           (
# 280 "/lib/tcc/include/tccdefs.h"
                            void
# 280 "/lib/tcc/include/tccdefs.h"
                                 *
# 280 "/lib/tcc/include/tccdefs.h"
                                  ,
# 280 "/lib/tcc/include/tccdefs.h"
                                    const
# 280 "/lib/tcc/include/tccdefs.h"
                                          void
# 280 "/lib/tcc/include/tccdefs.h"
                                              *
# 280 "/lib/tcc/include/tccdefs.h"
                                               ,
# 280 "/lib/tcc/include/tccdefs.h"
                                                 unsigned
# 280 "/lib/tcc/include/tccdefs.h"
                                                 long
# 280 "/lib/tcc/include/tccdefs.h"
                                                              )
# 280 "/lib/tcc/include/tccdefs.h"
    __asm__
# 280 "/lib/tcc/include/tccdefs.h"
    (
# 280 "/lib/tcc/include/tccdefs.h"
    "memmove"
# 280 "/lib/tcc/include/tccdefs.h"
    )
# 280 "/lib/tcc/include/tccdefs.h"
    ;
# 281 "/lib/tcc/include/tccdefs.h"
           void
# 281 "/lib/tcc/include/tccdefs.h"
               *
# 281 "/lib/tcc/include/tccdefs.h"
    __builtin_memset
# 281 "/lib/tcc/include/tccdefs.h"
                          (
# 281 "/lib/tcc/include/tccdefs.h"
                           void
# 281 "/lib/tcc/include/tccdefs.h"
                                *
# 281 "/lib/tcc/include/tccdefs.h"
                                 ,
# 281 "/lib/tcc/include/tccdefs.h"
                                   int
# 281 "/lib/tcc/include/tccdefs.h"
                                      ,
# 281 "/lib/tcc/include/tccdefs.h"
                                        unsigned
# 281 "/lib/tcc/include/tccdefs.h"
                                        long
# 281 "/lib/tcc/include/tccdefs.h"
                                                     )
# 281 "/lib/tcc/include/tccdefs.h"
    __asm__
# 281 "/lib/tcc/include/tccdefs.h"
    (
# 281 "/lib/tcc/include/tccdefs.h"
    "memset"
# 281 "/lib/tcc/include/tccdefs.h"
    )
# 281 "/lib/tcc/include/tccdefs.h"
    ;
# 282 "/lib/tcc/include/tccdefs.h"
           int
# 282 "/lib/tcc/include/tccdefs.h"
    __builtin_memcmp
# 282 "/lib/tcc/include/tccdefs.h"
                        (
# 282 "/lib/tcc/include/tccdefs.h"
                         const
# 282 "/lib/tcc/include/tccdefs.h"
                               void
# 282 "/lib/tcc/include/tccdefs.h"
                                    *
# 282 "/lib/tcc/include/tccdefs.h"
                                     ,
# 282 "/lib/tcc/include/tccdefs.h"
                                       const
# 282 "/lib/tcc/include/tccdefs.h"
                                             void
# 282 "/lib/tcc/include/tccdefs.h"
                                                 *
# 282 "/lib/tcc/include/tccdefs.h"
                                                  ,
# 282 "/lib/tcc/include/tccdefs.h"
                                                    unsigned
# 282 "/lib/tcc/include/tccdefs.h"
                                                    long
# 282 "/lib/tcc/include/tccdefs.h"
                                                                 )
# 282 "/lib/tcc/include/tccdefs.h"
    __asm__
# 282 "/lib/tcc/include/tccdefs.h"
    (
# 282 "/lib/tcc/include/tccdefs.h"
    "memcmp"
# 282 "/lib/tcc/include/tccdefs.h"
    )
# 282 "/lib/tcc/include/tccdefs.h"
    ;
# 283 "/lib/tcc/include/tccdefs.h"
           unsigned
# 283 "/lib/tcc/include/tccdefs.h"
           long
# 283 "/lib/tcc/include/tccdefs.h"
    __builtin_strlen
# 283 "/lib/tcc/include/tccdefs.h"
                                  (
# 283 "/lib/tcc/include/tccdefs.h"
                                   const
# 283 "/lib/tcc/include/tccdefs.h"
                                         char
# 283 "/lib/tcc/include/tccdefs.h"
                                              *
# 283 "/lib/tcc/include/tccdefs.h"
                                               )
# 283 "/lib/tcc/include/tccdefs.h"
    __asm__
# 283 "/lib/tcc/include/tccdefs.h"
    (
# 283 "/lib/tcc/include/tccdefs.h"
    "strlen"
# 283 "/lib/tcc/include/tccdefs.h"
    )
# 283 "/lib/tcc/include/tccdefs.h"
    ;
# 284 "/lib/tcc/include/tccdefs.h"
           char
# 284 "/lib/tcc/include/tccdefs.h"
               *
# 284 "/lib/tcc/include/tccdefs.h"
    __builtin_strcpy
# 284 "/lib/tcc/include/tccdefs.h"
                          (
# 284 "/lib/tcc/include/tccdefs.h"
                           char
# 284 "/lib/tcc/include/tccdefs.h"
                                *
# 284 "/lib/tcc/include/tccdefs.h"
                                 ,
# 284 "/lib/tcc/include/tccdefs.h"
                                   const
# 284 "/lib/tcc/include/tccdefs.h"
                                         char
# 284 "/lib/tcc/include/tccdefs.h"
                                              *
# 284 "/lib/tcc/include/tccdefs.h"
                                               )
# 284 "/lib/tcc/include/tccdefs.h"
    __asm__
# 284 "/lib/tcc/include/tccdefs.h"
    (
# 284 "/lib/tcc/include/tccdefs.h"
    "strcpy"
# 284 "/lib/tcc/include/tccdefs.h"
    )
# 284 "/lib/tcc/include/tccdefs.h"
    ;
# 285 "/lib/tcc/include/tccdefs.h"
           char
# 285 "/lib/tcc/include/tccdefs.h"
               *
# 285 "/lib/tcc/include/tccdefs.h"
    __builtin_strncpy
# 285 "/lib/tcc/include/tccdefs.h"
                           (
# 285 "/lib/tcc/include/tccdefs.h"
                            char
# 285 "/lib/tcc/include/tccdefs.h"
                                 *
# 285 "/lib/tcc/include/tccdefs.h"
                                  ,
# 285 "/lib/tcc/include/tccdefs.h"
                                    const
# 285 "/lib/tcc/include/tccdefs.h"
                                          char
# 285 "/lib/tcc/include/tccdefs.h"
                                              *
# 285 "/lib/tcc/include/tccdefs.h"
                                               ,
# 285 "/lib/tcc/include/tccdefs.h"
                                                 unsigned
# 285 "/lib/tcc/include/tccdefs.h"
                                                 long
# 285 "/lib/tcc/include/tccdefs.h"
                                                              )
# 285 "/lib/tcc/include/tccdefs.h"
    __asm__
# 285 "/lib/tcc/include/tccdefs.h"
    (
# 285 "/lib/tcc/include/tccdefs.h"
    "strncpy"
# 285 "/lib/tcc/include/tccdefs.h"
    )
# 285 "/lib/tcc/include/tccdefs.h"
    ;
# 286 "/lib/tcc/include/tccdefs.h"
           int
# 286 "/lib/tcc/include/tccdefs.h"
    __builtin_strcmp
# 286 "/lib/tcc/include/tccdefs.h"
                        (
# 286 "/lib/tcc/include/tccdefs.h"
                         const
# 286 "/lib/tcc/include/tccdefs.h"
                               char
# 286 "/lib/tcc/include/tccdefs.h"
                                   *
# 286 "/lib/tcc/include/tccdefs.h"
                                    ,
# 286 "/lib/tcc/include/tccdefs.h"
                                      const
# 286 "/lib/tcc/include/tccdefs.h"
                                            char
# 286 "/lib/tcc/include/tccdefs.h"
                                                *
# 286 "/lib/tcc/include/tccdefs.h"
                                                 )
# 286 "/lib/tcc/include/tccdefs.h"
    __asm__
# 286 "/lib/tcc/include/tccdefs.h"
    (
# 286 "/lib/tcc/include/tccdefs.h"
    "strcmp"
# 286 "/lib/tcc/include/tccdefs.h"
    )
# 286 "/lib/tcc/include/tccdefs.h"
    ;
# 287 "/lib/tcc/include/tccdefs.h"
           int
# 287 "/lib/tcc/include/tccdefs.h"
    __builtin_strncmp
# 287 "/lib/tcc/include/tccdefs.h"
                         (
# 287 "/lib/tcc/include/tccdefs.h"
                          const
# 287 "/lib/tcc/include/tccdefs.h"
                                char
# 287 "/lib/tcc/include/tccdefs.h"
                                    *
# 287 "/lib/tcc/include/tccdefs.h"
                                     ,
# 287 "/lib/tcc/include/tccdefs.h"
                                       const
# 287 "/lib/tcc/include/tccdefs.h"
                                             char
# 287 "/lib/tcc/include/tccdefs.h"
                                                 *
# 287 "/lib/tcc/include/tccdefs.h"
                                                  ,
# 287 "/lib/tcc/include/tccdefs.h"
                                                    unsigned
# 287 "/lib/tcc/include/tccdefs.h"
                                                    long
# 287 "/lib/tcc/include/tccdefs.h"
                                                                 )
# 287 "/lib/tcc/include/tccdefs.h"
    __asm__
# 287 "/lib/tcc/include/tccdefs.h"
    (
# 287 "/lib/tcc/include/tccdefs.h"
    "strncmp"
# 287 "/lib/tcc/include/tccdefs.h"
    )
# 287 "/lib/tcc/include/tccdefs.h"
    ;
# 288 "/lib/tcc/include/tccdefs.h"
           char
# 288 "/lib/tcc/include/tccdefs.h"
               *
# 288 "/lib/tcc/include/tccdefs.h"
    __builtin_strcat
# 288 "/lib/tcc/include/tccdefs.h"
                          (
# 288 "/lib/tcc/include/tccdefs.h"
                           char
# 288 "/lib/tcc/include/tccdefs.h"
                               *
# 288 "/lib/tcc/include/tccdefs.h"
                                ,
# 288 "/lib/tcc/include/tccdefs.h"
                                  const
# 288 "/lib/tcc/include/tccdefs.h"
                                        char
# 288 "/lib/tcc/include/tccdefs.h"
                                            *
# 288 "/lib/tcc/include/tccdefs.h"
                                             )
# 288 "/lib/tcc/include/tccdefs.h"
    __asm__
# 288 "/lib/tcc/include/tccdefs.h"
    (
# 288 "/lib/tcc/include/tccdefs.h"
    "strcat"
# 288 "/lib/tcc/include/tccdefs.h"
    )
# 288 "/lib/tcc/include/tccdefs.h"
    ;
# 289 "/lib/tcc/include/tccdefs.h"
           char
# 289 "/lib/tcc/include/tccdefs.h"
               *
# 289 "/lib/tcc/include/tccdefs.h"
    __builtin_strncat
# 289 "/lib/tcc/include/tccdefs.h"
                           (
# 289 "/lib/tcc/include/tccdefs.h"
                            char
# 289 "/lib/tcc/include/tccdefs.h"
                                *
# 289 "/lib/tcc/include/tccdefs.h"
                                 ,
# 289 "/lib/tcc/include/tccdefs.h"
                                   const
# 289 "/lib/tcc/include/tccdefs.h"
                                         char
# 289 "/lib/tcc/include/tccdefs.h"
                                             *
# 289 "/lib/tcc/include/tccdefs.h"
                                              ,
# 289 "/lib/tcc/include/tccdefs.h"
                                                unsigned
# 289 "/lib/tcc/include/tccdefs.h"
                                                long
# 289 "/lib/tcc/include/tccdefs.h"
                                                             )
# 289 "/lib/tcc/include/tccdefs.h"
    __asm__
# 289 "/lib/tcc/include/tccdefs.h"
    (
# 289 "/lib/tcc/include/tccdefs.h"
    "strncat"
# 289 "/lib/tcc/include/tccdefs.h"
    )
# 289 "/lib/tcc/include/tccdefs.h"
    ;
# 290 "/lib/tcc/include/tccdefs.h"
           char
# 290 "/lib/tcc/include/tccdefs.h"
               *
# 290 "/lib/tcc/include/tccdefs.h"
    __builtin_strchr
# 290 "/lib/tcc/include/tccdefs.h"
                          (
# 290 "/lib/tcc/include/tccdefs.h"
                           const
# 290 "/lib/tcc/include/tccdefs.h"
                                 char
# 290 "/lib/tcc/include/tccdefs.h"
                                     *
# 290 "/lib/tcc/include/tccdefs.h"
                                      ,
# 290 "/lib/tcc/include/tccdefs.h"
                                        int
# 290 "/lib/tcc/include/tccdefs.h"
                                           )
# 290 "/lib/tcc/include/tccdefs.h"
    __asm__
# 290 "/lib/tcc/include/tccdefs.h"
    (
# 290 "/lib/tcc/include/tccdefs.h"
    "strchr"
# 290 "/lib/tcc/include/tccdefs.h"
    )
# 290 "/lib/tcc/include/tccdefs.h"
    ;
# 291 "/lib/tcc/include/tccdefs.h"
           char
# 291 "/lib/tcc/include/tccdefs.h"
               *
# 291 "/lib/tcc/include/tccdefs.h"
    __builtin_strrchr
# 291 "/lib/tcc/include/tccdefs.h"
                           (
# 291 "/lib/tcc/include/tccdefs.h"
                            const
# 291 "/lib/tcc/include/tccdefs.h"
                                  char
# 291 "/lib/tcc/include/tccdefs.h"
                                      *
# 291 "/lib/tcc/include/tccdefs.h"
                                       ,
# 291 "/lib/tcc/include/tccdefs.h"
                                         int
# 291 "/lib/tcc/include/tccdefs.h"
                                            )
# 291 "/lib/tcc/include/tccdefs.h"
    __asm__
# 291 "/lib/tcc/include/tccdefs.h"
    (
# 291 "/lib/tcc/include/tccdefs.h"
    "strrchr"
# 291 "/lib/tcc/include/tccdefs.h"
    )
# 291 "/lib/tcc/include/tccdefs.h"
    ;
# 292 "/lib/tcc/include/tccdefs.h"
           char
# 292 "/lib/tcc/include/tccdefs.h"
               *
# 292 "/lib/tcc/include/tccdefs.h"
    __builtin_strdup
# 292 "/lib/tcc/include/tccdefs.h"
                          (
# 292 "/lib/tcc/include/tccdefs.h"
                           const
# 292 "/lib/tcc/include/tccdefs.h"
                                 char
# 292 "/lib/tcc/include/tccdefs.h"
                                     *
# 292 "/lib/tcc/include/tccdefs.h"
                                      )
# 292 "/lib/tcc/include/tccdefs.h"
    __asm__
# 292 "/lib/tcc/include/tccdefs.h"
    (
# 292 "/lib/tcc/include/tccdefs.h"
    "strdup"
# 292 "/lib/tcc/include/tccdefs.h"
    )
# 292 "/lib/tcc/include/tccdefs.h"
    ;
# 306 "/lib/tcc/include/tccdefs.h"
                  void
# 306 "/lib/tcc/include/tccdefs.h"
                      *
# 306 "/lib/tcc/include/tccdefs.h"
    __builtin_malloc
# 306 "/lib/tcc/include/tccdefs.h"
                                 (
# 306 "/lib/tcc/include/tccdefs.h"
                                  unsigned
# 306 "/lib/tcc/include/tccdefs.h"
                                  long
# 306 "/lib/tcc/include/tccdefs.h"
                                               )
# 306 "/lib/tcc/include/tccdefs.h"
    __asm__
# 306 "/lib/tcc/include/tccdefs.h"
    (
# 306 "/lib/tcc/include/tccdefs.h"
    "malloc"
# 306 "/lib/tcc/include/tccdefs.h"
    )
# 306 "/lib/tcc/include/tccdefs.h"
    ;
# 307 "/lib/tcc/include/tccdefs.h"
                  void
# 307 "/lib/tcc/include/tccdefs.h"
                      *
# 307 "/lib/tcc/include/tccdefs.h"
    __builtin_realloc
# 307 "/lib/tcc/include/tccdefs.h"
                                  (
# 307 "/lib/tcc/include/tccdefs.h"
                                   void
# 307 "/lib/tcc/include/tccdefs.h"
                                        *
# 307 "/lib/tcc/include/tccdefs.h"
                                         ,
# 307 "/lib/tcc/include/tccdefs.h"
                                           unsigned
# 307 "/lib/tcc/include/tccdefs.h"
                                           long
# 307 "/lib/tcc/include/tccdefs.h"
                                                        )
# 307 "/lib/tcc/include/tccdefs.h"
    __asm__
# 307 "/lib/tcc/include/tccdefs.h"
    (
# 307 "/lib/tcc/include/tccdefs.h"
    "realloc"
# 307 "/lib/tcc/include/tccdefs.h"
    )
# 307 "/lib/tcc/include/tccdefs.h"
    ;
# 308 "/lib/tcc/include/tccdefs.h"
                  void
# 308 "/lib/tcc/include/tccdefs.h"
                      *
# 308 "/lib/tcc/include/tccdefs.h"
    __builtin_calloc
# 308 "/lib/tcc/include/tccdefs.h"
                                 (
# 308 "/lib/tcc/include/tccdefs.h"
                                  unsigned
# 308 "/lib/tcc/include/tccdefs.h"
                                  long
# 308 "/lib/tcc/include/tccdefs.h"
                                               ,
# 308 "/lib/tcc/include/tccdefs.h"
                                                 unsigned
# 308 "/lib/tcc/include/tccdefs.h"
                                                 long
# 308 "/lib/tcc/include/tccdefs.h"
                                                              )
# 308 "/lib/tcc/include/tccdefs.h"
    __asm__
# 308 "/lib/tcc/include/tccdefs.h"
    (
# 308 "/lib/tcc/include/tccdefs.h"
    "calloc"
# 308 "/lib/tcc/include/tccdefs.h"
    )
# 308 "/lib/tcc/include/tccdefs.h"
    ;
# 309 "/lib/tcc/include/tccdefs.h"
                  void
# 309 "/lib/tcc/include/tccdefs.h"
                      *
# 309 "/lib/tcc/include/tccdefs.h"
    __builtin_memalign
# 309 "/lib/tcc/include/tccdefs.h"
                                   (
# 309 "/lib/tcc/include/tccdefs.h"
                                    unsigned
# 309 "/lib/tcc/include/tccdefs.h"
                                    long
# 309 "/lib/tcc/include/tccdefs.h"
                                                 ,
# 309 "/lib/tcc/include/tccdefs.h"
                                                   unsigned
# 309 "/lib/tcc/include/tccdefs.h"
                                                   long
# 309 "/lib/tcc/include/tccdefs.h"
                                                                )
# 309 "/lib/tcc/include/tccdefs.h"
    __asm__
# 309 "/lib/tcc/include/tccdefs.h"
    (
# 309 "/lib/tcc/include/tccdefs.h"
    "memalign"
# 309 "/lib/tcc/include/tccdefs.h"
    )
# 309 "/lib/tcc/include/tccdefs.h"
    ;
# 310 "/lib/tcc/include/tccdefs.h"
                  void
# 310 "/lib/tcc/include/tccdefs.h"
    __builtin_free
# 310 "/lib/tcc/include/tccdefs.h"
                              (
# 310 "/lib/tcc/include/tccdefs.h"
                               void
# 310 "/lib/tcc/include/tccdefs.h"
                                   *
# 310 "/lib/tcc/include/tccdefs.h"
                                    )
# 310 "/lib/tcc/include/tccdefs.h"
    __asm__
# 310 "/lib/tcc/include/tccdefs.h"
    (
# 310 "/lib/tcc/include/tccdefs.h"
    "free"
# 310 "/lib/tcc/include/tccdefs.h"
    )
# 310 "/lib/tcc/include/tccdefs.h"
    ;
# 311 "/lib/tcc/include/tccdefs.h"
           void
# 311 "/lib/tcc/include/tccdefs.h"
               *
# 311 "/lib/tcc/include/tccdefs.h"
    __builtin_alloca
# 311 "/lib/tcc/include/tccdefs.h"
                          (
# 311 "/lib/tcc/include/tccdefs.h"
                           unsigned
# 311 "/lib/tcc/include/tccdefs.h"
                           long
# 311 "/lib/tcc/include/tccdefs.h"
                                        )
# 311 "/lib/tcc/include/tccdefs.h"
    __asm__
# 311 "/lib/tcc/include/tccdefs.h"
    (
# 311 "/lib/tcc/include/tccdefs.h"
    "alloca"
# 311 "/lib/tcc/include/tccdefs.h"
    )
# 311 "/lib/tcc/include/tccdefs.h"
    ;
# 312 "/lib/tcc/include/tccdefs.h"
    void
# 312 "/lib/tcc/include/tccdefs.h"
         *
# 312 "/lib/tcc/include/tccdefs.h"
          alloca
# 312 "/lib/tcc/include/tccdefs.h"
                (
# 312 "/lib/tcc/include/tccdefs.h"
                 unsigned
# 312 "/lib/tcc/include/tccdefs.h"
                 long
# 312 "/lib/tcc/include/tccdefs.h"
                              )
# 312 "/lib/tcc/include/tccdefs.h"
                               ;
# 313 "/lib/tcc/include/tccdefs.h"
              void
# 313 "/lib/tcc/include/tccdefs.h"
    __builtin_abort
# 313 "/lib/tcc/include/tccdefs.h"
                           (
# 313 "/lib/tcc/include/tccdefs.h"
                            void
# 313 "/lib/tcc/include/tccdefs.h"
                                )
# 313 "/lib/tcc/include/tccdefs.h"
    __asm__
# 313 "/lib/tcc/include/tccdefs.h"
    (
# 313 "/lib/tcc/include/tccdefs.h"
    "abort"
# 313 "/lib/tcc/include/tccdefs.h"
    )
# 313 "/lib/tcc/include/tccdefs.h"
    ;
# 330 "/lib/tcc/include/tccdefs.h"
    int
# 330 "/lib/tcc/include/tccdefs.h"
    __builtin_ffs
# 330 "/lib/tcc/include/tccdefs.h"
    (
# 330 "/lib/tcc/include/tccdefs.h"
    int
# 330 "/lib/tcc/include/tccdefs.h"
    )
# 330 "/lib/tcc/include/tccdefs.h"
    ;
# 330 "/lib/tcc/include/tccdefs.h"
    int
# 330 "/lib/tcc/include/tccdefs.h"
    __builtin_ffsl
# 330 "/lib/tcc/include/tccdefs.h"
    (
# 330 "/lib/tcc/include/tccdefs.h"
    long
# 330 "/lib/tcc/include/tccdefs.h"
    )
# 330 "/lib/tcc/include/tccdefs.h"
    ;
# 330 "/lib/tcc/include/tccdefs.h"
    int
# 330 "/lib/tcc/include/tccdefs.h"
    __builtin_ffsll
# 330 "/lib/tcc/include/tccdefs.h"
    (
# 330 "/lib/tcc/include/tccdefs.h"
    long
# 330 "/lib/tcc/include/tccdefs.h"
    long
# 330 "/lib/tcc/include/tccdefs.h"
    )
# 330 "/lib/tcc/include/tccdefs.h"
    ;
# 331 "/lib/tcc/include/tccdefs.h"
    int
# 331 "/lib/tcc/include/tccdefs.h"
    __builtin_clz
# 331 "/lib/tcc/include/tccdefs.h"
    (
# 331 "/lib/tcc/include/tccdefs.h"
                          unsigned
# 331 "/lib/tcc/include/tccdefs.h"
    int
# 331 "/lib/tcc/include/tccdefs.h"
    )
# 331 "/lib/tcc/include/tccdefs.h"
    ;
# 331 "/lib/tcc/include/tccdefs.h"
    int
# 331 "/lib/tcc/include/tccdefs.h"
    __builtin_clzl
# 331 "/lib/tcc/include/tccdefs.h"
    (
# 331 "/lib/tcc/include/tccdefs.h"
                          unsigned
# 331 "/lib/tcc/include/tccdefs.h"
    long
# 331 "/lib/tcc/include/tccdefs.h"
    )
# 331 "/lib/tcc/include/tccdefs.h"
    ;
# 331 "/lib/tcc/include/tccdefs.h"
    int
# 331 "/lib/tcc/include/tccdefs.h"
    __builtin_clzll
# 331 "/lib/tcc/include/tccdefs.h"
    (
# 331 "/lib/tcc/include/tccdefs.h"
                          unsigned
# 331 "/lib/tcc/include/tccdefs.h"
    long
# 331 "/lib/tcc/include/tccdefs.h"
    long
# 331 "/lib/tcc/include/tccdefs.h"
    )
# 331 "/lib/tcc/include/tccdefs.h"
    ;
# 332 "/lib/tcc/include/tccdefs.h"
    int
# 332 "/lib/tcc/include/tccdefs.h"
    __builtin_ctz
# 332 "/lib/tcc/include/tccdefs.h"
    (
# 332 "/lib/tcc/include/tccdefs.h"
                          unsigned
# 332 "/lib/tcc/include/tccdefs.h"
    int
# 332 "/lib/tcc/include/tccdefs.h"
    )
# 332 "/lib/tcc/include/tccdefs.h"
    ;
# 332 "/lib/tcc/include/tccdefs.h"
    int
# 332 "/lib/tcc/include/tccdefs.h"
    __builtin_ctzl
# 332 "/lib/tcc/include/tccdefs.h"
    (
# 332 "/lib/tcc/include/tccdefs.h"
                          unsigned
# 332 "/lib/tcc/include/tccdefs.h"
    long
# 332 "/lib/tcc/include/tccdefs.h"
    )
# 332 "/lib/tcc/include/tccdefs.h"
    ;
# 332 "/lib/tcc/include/tccdefs.h"
    int
# 332 "/lib/tcc/include/tccdefs.h"
    __builtin_ctzll
# 332 "/lib/tcc/include/tccdefs.h"
    (
# 332 "/lib/tcc/include/tccdefs.h"
                          unsigned
# 332 "/lib/tcc/include/tccdefs.h"
    long
# 332 "/lib/tcc/include/tccdefs.h"
    long
# 332 "/lib/tcc/include/tccdefs.h"
    )
# 332 "/lib/tcc/include/tccdefs.h"
    ;
# 333 "/lib/tcc/include/tccdefs.h"
    int
# 333 "/lib/tcc/include/tccdefs.h"
    __builtin_clrsb
# 333 "/lib/tcc/include/tccdefs.h"
    (
# 333 "/lib/tcc/include/tccdefs.h"
    int
# 333 "/lib/tcc/include/tccdefs.h"
    )
# 333 "/lib/tcc/include/tccdefs.h"
    ;
# 333 "/lib/tcc/include/tccdefs.h"
    int
# 333 "/lib/tcc/include/tccdefs.h"
    __builtin_clrsbl
# 333 "/lib/tcc/include/tccdefs.h"
    (
# 333 "/lib/tcc/include/tccdefs.h"
    long
# 333 "/lib/tcc/include/tccdefs.h"
    )
# 333 "/lib/tcc/include/tccdefs.h"
    ;
# 333 "/lib/tcc/include/tccdefs.h"
    int
# 333 "/lib/tcc/include/tccdefs.h"
    __builtin_clrsbll
# 333 "/lib/tcc/include/tccdefs.h"
    (
# 333 "/lib/tcc/include/tccdefs.h"
    long
# 333 "/lib/tcc/include/tccdefs.h"
    long
# 333 "/lib/tcc/include/tccdefs.h"
    )
# 333 "/lib/tcc/include/tccdefs.h"
    ;
# 334 "/lib/tcc/include/tccdefs.h"
    int
# 334 "/lib/tcc/include/tccdefs.h"
    __builtin_popcount
# 334 "/lib/tcc/include/tccdefs.h"
    (
# 334 "/lib/tcc/include/tccdefs.h"
                               unsigned
# 334 "/lib/tcc/include/tccdefs.h"
    int
# 334 "/lib/tcc/include/tccdefs.h"
    )
# 334 "/lib/tcc/include/tccdefs.h"
    ;
# 334 "/lib/tcc/include/tccdefs.h"
    int
# 334 "/lib/tcc/include/tccdefs.h"
    __builtin_popcountl
# 334 "/lib/tcc/include/tccdefs.h"
    (
# 334 "/lib/tcc/include/tccdefs.h"
                               unsigned
# 334 "/lib/tcc/include/tccdefs.h"
    long
# 334 "/lib/tcc/include/tccdefs.h"
    )
# 334 "/lib/tcc/include/tccdefs.h"
    ;
# 334 "/lib/tcc/include/tccdefs.h"
    int
# 334 "/lib/tcc/include/tccdefs.h"
    __builtin_popcountll
# 334 "/lib/tcc/include/tccdefs.h"
    (
# 334 "/lib/tcc/include/tccdefs.h"
                               unsigned
# 334 "/lib/tcc/include/tccdefs.h"
    long
# 334 "/lib/tcc/include/tccdefs.h"
    long
# 334 "/lib/tcc/include/tccdefs.h"
    )
# 334 "/lib/tcc/include/tccdefs.h"
    ;
# 335 "/lib/tcc/include/tccdefs.h"
    int
# 335 "/lib/tcc/include/tccdefs.h"
    __builtin_parity
# 335 "/lib/tcc/include/tccdefs.h"
    (
# 335 "/lib/tcc/include/tccdefs.h"
                             unsigned
# 335 "/lib/tcc/include/tccdefs.h"
    int
# 335 "/lib/tcc/include/tccdefs.h"
    )
# 335 "/lib/tcc/include/tccdefs.h"
    ;
# 335 "/lib/tcc/include/tccdefs.h"
    int
# 335 "/lib/tcc/include/tccdefs.h"
    __builtin_parityl
# 335 "/lib/tcc/include/tccdefs.h"
    (
# 335 "/lib/tcc/include/tccdefs.h"
                             unsigned
# 335 "/lib/tcc/include/tccdefs.h"
    long
# 335 "/lib/tcc/include/tccdefs.h"
    )
# 335 "/lib/tcc/include/tccdefs.h"
    ;
# 335 "/lib/tcc/include/tccdefs.h"
    int
# 335 "/lib/tcc/include/tccdefs.h"
    __builtin_parityll
# 335 "/lib/tcc/include/tccdefs.h"
    (
# 335 "/lib/tcc/include/tccdefs.h"
                             unsigned
# 335 "/lib/tcc/include/tccdefs.h"
    long
# 335 "/lib/tcc/include/tccdefs.h"
    long
# 335 "/lib/tcc/include/tccdefs.h"
    )
# 335 "/lib/tcc/include/tccdefs.h"
    ;
# 1 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-csem-repin/results/csem/gnu-attributes/initial/ordinary.c"
int
# 1 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-csem-repin/results/csem/gnu-attributes/initial/ordinary.c"
    main
# 1 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-csem-repin/results/csem/gnu-attributes/initial/ordinary.c"
        (
# 1 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-csem-repin/results/csem/gnu-attributes/initial/ordinary.c"
         void
# 1 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-csem-repin/results/csem/gnu-attributes/initial/ordinary.c"
             )
# 1 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-csem-repin/results/csem/gnu-attributes/initial/ordinary.c"
              {
# 1 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-csem-repin/results/csem/gnu-attributes/initial/ordinary.c"
               return
# 1 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-csem-repin/results/csem/gnu-attributes/initial/ordinary.c"
                      0
# 1 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-csem-repin/results/csem/gnu-attributes/initial/ordinary.c"
                       ;
# 1 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-csem-repin/results/csem/gnu-attributes/initial/ordinary.c"
                        }

