# 5 "/runtime/libc/include/stdarg.h"
typedef
# 5 "/runtime/libc/include/stdarg.h"
        int
# 5 "/runtime/libc/include/stdarg.h"
            va_list
# 5 "/runtime/libc/include/stdarg.h"
                   ;
# 6 "/runtime/libc/include/stdio.h"
typedef
# 6 "/runtime/libc/include/stdio.h"
        __cerbty_size_t
# 6 "/runtime/libc/include/stdio.h"
                        size_t
# 6 "/runtime/libc/include/stdio.h"
                              ;
# 8 "/runtime/libc/include/stdio.h"
typedef
# 8 "/runtime/libc/include/stdio.h"
        struct
# 8 "/runtime/libc/include/stdio.h"
               _IO_FILE
# 8 "/runtime/libc/include/stdio.h"
                        FILE
# 8 "/runtime/libc/include/stdio.h"
                            ;
# 16 "/runtime/libc/include/stdio.h"
typedef
# 16 "/runtime/libc/include/stdio.h"
        long
# 16 "/runtime/libc/include/stdio.h"
             long
# 16 "/runtime/libc/include/stdio.h"
                  fpos_t
# 16 "/runtime/libc/include/stdio.h"
                        ;
# 42 "/runtime/libc/include/stdio.h"
extern
# 42 "/runtime/libc/include/stdio.h"
       FILE
# 42 "/runtime/libc/include/stdio.h"
            *
# 42 "/runtime/libc/include/stdio.h"
             const
# 42 "/runtime/libc/include/stdio.h"
                   __stdout
# 42 "/runtime/libc/include/stdio.h"
                           ;
# 43 "/runtime/libc/include/stdio.h"
extern
# 43 "/runtime/libc/include/stdio.h"
       FILE
# 43 "/runtime/libc/include/stdio.h"
            *
# 43 "/runtime/libc/include/stdio.h"
             const
# 43 "/runtime/libc/include/stdio.h"
                   __stderr
# 43 "/runtime/libc/include/stdio.h"
                           ;
# 44 "/runtime/libc/include/stdio.h"
extern
# 44 "/runtime/libc/include/stdio.h"
       FILE
# 44 "/runtime/libc/include/stdio.h"
            *
# 44 "/runtime/libc/include/stdio.h"
             const
# 44 "/runtime/libc/include/stdio.h"
                   __stdin
# 44 "/runtime/libc/include/stdio.h"
                          ;
# 54 "/runtime/libc/include/stdio.h"
int
# 54 "/runtime/libc/include/stdio.h"
    remove
# 54 "/runtime/libc/include/stdio.h"
          (
# 54 "/runtime/libc/include/stdio.h"
           const
# 54 "/runtime/libc/include/stdio.h"
                 char
# 54 "/runtime/libc/include/stdio.h"
                      *
# 54 "/runtime/libc/include/stdio.h"
                       filename
# 54 "/runtime/libc/include/stdio.h"
                               )
# 54 "/runtime/libc/include/stdio.h"
                                ;
# 55 "/runtime/libc/include/stdio.h"
int
# 55 "/runtime/libc/include/stdio.h"
    rename
# 55 "/runtime/libc/include/stdio.h"
          (
# 55 "/runtime/libc/include/stdio.h"
           const
# 55 "/runtime/libc/include/stdio.h"
                 char
# 55 "/runtime/libc/include/stdio.h"
                      *
# 55 "/runtime/libc/include/stdio.h"
                       old
# 55 "/runtime/libc/include/stdio.h"
                          ,
# 55 "/runtime/libc/include/stdio.h"
                            const
# 55 "/runtime/libc/include/stdio.h"
                                  char
# 55 "/runtime/libc/include/stdio.h"
                                       *
# 55 "/runtime/libc/include/stdio.h"
                                        new
# 55 "/runtime/libc/include/stdio.h"
                                           )
# 55 "/runtime/libc/include/stdio.h"
                                            ;
# 56 "/runtime/libc/include/stdio.h"
FILE
# 56 "/runtime/libc/include/stdio.h"
     *
# 56 "/runtime/libc/include/stdio.h"
      tmpfile
# 56 "/runtime/libc/include/stdio.h"
             (
# 56 "/runtime/libc/include/stdio.h"
              void
# 56 "/runtime/libc/include/stdio.h"
                  )
# 56 "/runtime/libc/include/stdio.h"
                   ;
# 57 "/runtime/libc/include/stdio.h"
char
# 57 "/runtime/libc/include/stdio.h"
     *
# 57 "/runtime/libc/include/stdio.h"
      tmpnam
# 57 "/runtime/libc/include/stdio.h"
            (
# 57 "/runtime/libc/include/stdio.h"
             char
# 57 "/runtime/libc/include/stdio.h"
                  *
# 57 "/runtime/libc/include/stdio.h"
                   s
# 57 "/runtime/libc/include/stdio.h"
                    )
# 57 "/runtime/libc/include/stdio.h"
                     ;
# 59 "/runtime/libc/include/stdio.h"
int
# 59 "/runtime/libc/include/stdio.h"
    fclose
# 59 "/runtime/libc/include/stdio.h"
          (
# 59 "/runtime/libc/include/stdio.h"
           FILE
# 59 "/runtime/libc/include/stdio.h"
                *
# 59 "/runtime/libc/include/stdio.h"
                 stream
# 59 "/runtime/libc/include/stdio.h"
                       )
# 59 "/runtime/libc/include/stdio.h"
                        ;
# 60 "/runtime/libc/include/stdio.h"
int
# 60 "/runtime/libc/include/stdio.h"
    fflush
# 60 "/runtime/libc/include/stdio.h"
          (
# 60 "/runtime/libc/include/stdio.h"
           FILE
# 60 "/runtime/libc/include/stdio.h"
                *
# 60 "/runtime/libc/include/stdio.h"
                 stream
# 60 "/runtime/libc/include/stdio.h"
                       )
# 60 "/runtime/libc/include/stdio.h"
                        ;
# 61 "/runtime/libc/include/stdio.h"
FILE
# 61 "/runtime/libc/include/stdio.h"
     *
# 61 "/runtime/libc/include/stdio.h"
      fopen
# 61 "/runtime/libc/include/stdio.h"
           (
# 61 "/runtime/libc/include/stdio.h"
            const
# 61 "/runtime/libc/include/stdio.h"
                  char
# 61 "/runtime/libc/include/stdio.h"
                       *
# 61 "/runtime/libc/include/stdio.h"
                         restrict
# 61 "/runtime/libc/include/stdio.h"
                                  filename
# 61 "/runtime/libc/include/stdio.h"
                                          ,
# 61 "/runtime/libc/include/stdio.h"
                                            const
# 61 "/runtime/libc/include/stdio.h"
                                                  char
# 61 "/runtime/libc/include/stdio.h"
                                                       *
# 61 "/runtime/libc/include/stdio.h"
                                                         restrict
# 61 "/runtime/libc/include/stdio.h"
                                                                  mode
# 61 "/runtime/libc/include/stdio.h"
                                                                      )
# 61 "/runtime/libc/include/stdio.h"
                                                                       ;
# 62 "/runtime/libc/include/stdio.h"
FILE
# 62 "/runtime/libc/include/stdio.h"
     *
# 62 "/runtime/libc/include/stdio.h"
      freopen
# 62 "/runtime/libc/include/stdio.h"
             (
# 62 "/runtime/libc/include/stdio.h"
              const
# 62 "/runtime/libc/include/stdio.h"
                    char
# 62 "/runtime/libc/include/stdio.h"
                         *
# 62 "/runtime/libc/include/stdio.h"
                           restrict
# 62 "/runtime/libc/include/stdio.h"
                                    filename
# 62 "/runtime/libc/include/stdio.h"
                                            ,
# 62 "/runtime/libc/include/stdio.h"
                                              const
# 62 "/runtime/libc/include/stdio.h"
                                                    char
# 62 "/runtime/libc/include/stdio.h"
                                                         *
# 62 "/runtime/libc/include/stdio.h"
                                                           restrict
# 62 "/runtime/libc/include/stdio.h"
                                                                    mode
# 62 "/runtime/libc/include/stdio.h"
                                                                        ,
# 62 "/runtime/libc/include/stdio.h"
                                                                          FILE
# 62 "/runtime/libc/include/stdio.h"
                                                                               *
# 62 "/runtime/libc/include/stdio.h"
                                                                                 restrict
# 62 "/runtime/libc/include/stdio.h"
                                                                                          stream
# 62 "/runtime/libc/include/stdio.h"
                                                                                                )
# 62 "/runtime/libc/include/stdio.h"
                                                                                                 ;
# 64 "/runtime/libc/include/stdio.h"
void
# 64 "/runtime/libc/include/stdio.h"
     setbuf
# 64 "/runtime/libc/include/stdio.h"
           (
# 64 "/runtime/libc/include/stdio.h"
            FILE
# 64 "/runtime/libc/include/stdio.h"
                 *
# 64 "/runtime/libc/include/stdio.h"
                   restrict
# 64 "/runtime/libc/include/stdio.h"
                            stream
# 64 "/runtime/libc/include/stdio.h"
                                  ,
# 64 "/runtime/libc/include/stdio.h"
                                    char
# 64 "/runtime/libc/include/stdio.h"
                                         *
# 64 "/runtime/libc/include/stdio.h"
                                           restrict
# 64 "/runtime/libc/include/stdio.h"
                                                    buf
# 64 "/runtime/libc/include/stdio.h"
                                                       )
# 64 "/runtime/libc/include/stdio.h"
                                                        ;
# 65 "/runtime/libc/include/stdio.h"
int
# 65 "/runtime/libc/include/stdio.h"
    setvbuf
# 65 "/runtime/libc/include/stdio.h"
           (
# 65 "/runtime/libc/include/stdio.h"
            FILE
# 65 "/runtime/libc/include/stdio.h"
                 *
# 65 "/runtime/libc/include/stdio.h"
                   restrict
# 65 "/runtime/libc/include/stdio.h"
                            stream
# 65 "/runtime/libc/include/stdio.h"
                                  ,
# 65 "/runtime/libc/include/stdio.h"
                                    char
# 65 "/runtime/libc/include/stdio.h"
                                         *
# 65 "/runtime/libc/include/stdio.h"
                                           restrict
# 65 "/runtime/libc/include/stdio.h"
                                                    buf
# 65 "/runtime/libc/include/stdio.h"
                                                       ,
# 65 "/runtime/libc/include/stdio.h"
                                                         int
# 65 "/runtime/libc/include/stdio.h"
                                                             mode
# 65 "/runtime/libc/include/stdio.h"
                                                                 ,
# 65 "/runtime/libc/include/stdio.h"
                                                                   size_t
# 65 "/runtime/libc/include/stdio.h"
                                                                          size
# 65 "/runtime/libc/include/stdio.h"
                                                                              )
# 65 "/runtime/libc/include/stdio.h"
                                                                               ;
# 67 "/runtime/libc/include/stdio.h"
int
# 67 "/runtime/libc/include/stdio.h"
    fprintf
# 67 "/runtime/libc/include/stdio.h"
           (
# 67 "/runtime/libc/include/stdio.h"
            FILE
# 67 "/runtime/libc/include/stdio.h"
                 *
# 67 "/runtime/libc/include/stdio.h"
                   restrict
# 67 "/runtime/libc/include/stdio.h"
                            stream
# 67 "/runtime/libc/include/stdio.h"
                                  ,
# 67 "/runtime/libc/include/stdio.h"
                                    const
# 67 "/runtime/libc/include/stdio.h"
                                          char
# 67 "/runtime/libc/include/stdio.h"
                                               *
# 67 "/runtime/libc/include/stdio.h"
                                                 restrict
# 67 "/runtime/libc/include/stdio.h"
                                                          fmt
# 67 "/runtime/libc/include/stdio.h"
                                                             ,
# 67 "/runtime/libc/include/stdio.h"
                                                               ...
# 67 "/runtime/libc/include/stdio.h"
                                                                  )
# 67 "/runtime/libc/include/stdio.h"
                                                                   ;
# 68 "/runtime/libc/include/stdio.h"
int
# 68 "/runtime/libc/include/stdio.h"
    fscanf
# 68 "/runtime/libc/include/stdio.h"
          (
# 68 "/runtime/libc/include/stdio.h"
           FILE
# 68 "/runtime/libc/include/stdio.h"
                *
# 68 "/runtime/libc/include/stdio.h"
                  restrict
# 68 "/runtime/libc/include/stdio.h"
                           stream
# 68 "/runtime/libc/include/stdio.h"
                                 ,
# 68 "/runtime/libc/include/stdio.h"
                                   const
# 68 "/runtime/libc/include/stdio.h"
                                         char
# 68 "/runtime/libc/include/stdio.h"
                                              *
# 68 "/runtime/libc/include/stdio.h"
                                                restrict
# 68 "/runtime/libc/include/stdio.h"
                                                         fmt
# 68 "/runtime/libc/include/stdio.h"
                                                            ,
# 68 "/runtime/libc/include/stdio.h"
                                                              ...
# 68 "/runtime/libc/include/stdio.h"
                                                                 )
# 68 "/runtime/libc/include/stdio.h"
                                                                  ;
# 70 "/runtime/libc/include/stdio.h"
int
# 70 "/runtime/libc/include/stdio.h"
    printf
# 70 "/runtime/libc/include/stdio.h"
          (
# 70 "/runtime/libc/include/stdio.h"
           const
# 70 "/runtime/libc/include/stdio.h"
                 char
# 70 "/runtime/libc/include/stdio.h"
                      *
# 70 "/runtime/libc/include/stdio.h"
                        restrict
# 70 "/runtime/libc/include/stdio.h"
                                 fmt
# 70 "/runtime/libc/include/stdio.h"
                                    ,
# 70 "/runtime/libc/include/stdio.h"
                                      ...
# 70 "/runtime/libc/include/stdio.h"
                                         )
# 70 "/runtime/libc/include/stdio.h"
                                          ;
# 71 "/runtime/libc/include/stdio.h"
int
# 71 "/runtime/libc/include/stdio.h"
    scanf
# 71 "/runtime/libc/include/stdio.h"
         (
# 71 "/runtime/libc/include/stdio.h"
          const
# 71 "/runtime/libc/include/stdio.h"
                char
# 71 "/runtime/libc/include/stdio.h"
                     *
# 71 "/runtime/libc/include/stdio.h"
                       restrict
# 71 "/runtime/libc/include/stdio.h"
                                fmt
# 71 "/runtime/libc/include/stdio.h"
                                   ,
# 71 "/runtime/libc/include/stdio.h"
                                     ...
# 71 "/runtime/libc/include/stdio.h"
                                        )
# 71 "/runtime/libc/include/stdio.h"
                                         ;
# 77 "/runtime/libc/include/stdio.h"
int
# 77 "/runtime/libc/include/stdio.h"
    snprintf
# 77 "/runtime/libc/include/stdio.h"
            (
# 77 "/runtime/libc/include/stdio.h"
             char
# 77 "/runtime/libc/include/stdio.h"
                  *
# 77 "/runtime/libc/include/stdio.h"
                    restrict
# 77 "/runtime/libc/include/stdio.h"
                             s
# 77 "/runtime/libc/include/stdio.h"
                              ,
# 77 "/runtime/libc/include/stdio.h"
                                size_t
# 77 "/runtime/libc/include/stdio.h"
                                       n
# 77 "/runtime/libc/include/stdio.h"
                                        ,
# 77 "/runtime/libc/include/stdio.h"
                                          const
# 77 "/runtime/libc/include/stdio.h"
                                                char
# 77 "/runtime/libc/include/stdio.h"
                                                     *
# 77 "/runtime/libc/include/stdio.h"
                                                       restrict
# 77 "/runtime/libc/include/stdio.h"
                                                                fmt
# 77 "/runtime/libc/include/stdio.h"
                                                                   ,
# 77 "/runtime/libc/include/stdio.h"
                                                                     ...
# 77 "/runtime/libc/include/stdio.h"
                                                                        )
# 77 "/runtime/libc/include/stdio.h"
                                                                         ;
# 78 "/runtime/libc/include/stdio.h"
int
# 78 "/runtime/libc/include/stdio.h"
    sprintf
# 78 "/runtime/libc/include/stdio.h"
           (
# 78 "/runtime/libc/include/stdio.h"
            char
# 78 "/runtime/libc/include/stdio.h"
                 *
# 78 "/runtime/libc/include/stdio.h"
                   restrict
# 78 "/runtime/libc/include/stdio.h"
                            s
# 78 "/runtime/libc/include/stdio.h"
                             ,
# 78 "/runtime/libc/include/stdio.h"
                               const
# 78 "/runtime/libc/include/stdio.h"
                                     char
# 78 "/runtime/libc/include/stdio.h"
                                          *
# 78 "/runtime/libc/include/stdio.h"
                                            restrict
# 78 "/runtime/libc/include/stdio.h"
                                                     fmt
# 78 "/runtime/libc/include/stdio.h"
                                                        ,
# 78 "/runtime/libc/include/stdio.h"
                                                          ...
# 78 "/runtime/libc/include/stdio.h"
                                                             )
# 78 "/runtime/libc/include/stdio.h"
                                                              ;
# 79 "/runtime/libc/include/stdio.h"
int
# 79 "/runtime/libc/include/stdio.h"
    sscanf
# 79 "/runtime/libc/include/stdio.h"
          (
# 79 "/runtime/libc/include/stdio.h"
           const
# 79 "/runtime/libc/include/stdio.h"
                 char
# 79 "/runtime/libc/include/stdio.h"
                      *
# 79 "/runtime/libc/include/stdio.h"
                        restrict
# 79 "/runtime/libc/include/stdio.h"
                                 s
# 79 "/runtime/libc/include/stdio.h"
                                  ,
# 79 "/runtime/libc/include/stdio.h"
                                    const
# 79 "/runtime/libc/include/stdio.h"
                                          char
# 79 "/runtime/libc/include/stdio.h"
                                               *
# 79 "/runtime/libc/include/stdio.h"
                                                 restrict
# 79 "/runtime/libc/include/stdio.h"
                                                          fmt
# 79 "/runtime/libc/include/stdio.h"
                                                             ,
# 79 "/runtime/libc/include/stdio.h"
                                                               ...
# 79 "/runtime/libc/include/stdio.h"
                                                                  )
# 79 "/runtime/libc/include/stdio.h"
                                                                   ;
# 81 "/runtime/libc/include/stdio.h"
int
# 81 "/runtime/libc/include/stdio.h"
    vfprintf
# 81 "/runtime/libc/include/stdio.h"
            (
# 81 "/runtime/libc/include/stdio.h"
             FILE
# 81 "/runtime/libc/include/stdio.h"
                  *
# 81 "/runtime/libc/include/stdio.h"
                    restrict
# 81 "/runtime/libc/include/stdio.h"
                             stream
# 81 "/runtime/libc/include/stdio.h"
                                   ,
# 81 "/runtime/libc/include/stdio.h"
                                     const
# 81 "/runtime/libc/include/stdio.h"
                                           char
# 81 "/runtime/libc/include/stdio.h"
                                                *
# 81 "/runtime/libc/include/stdio.h"
                                                  restrict
# 81 "/runtime/libc/include/stdio.h"
                                                           fmt
# 81 "/runtime/libc/include/stdio.h"
                                                              ,
# 81 "/runtime/libc/include/stdio.h"
                                                                va_list
# 81 "/runtime/libc/include/stdio.h"
                                                                        arg
# 81 "/runtime/libc/include/stdio.h"
                                                                           )
# 81 "/runtime/libc/include/stdio.h"
                                                                            ;
# 82 "/runtime/libc/include/stdio.h"
int
# 82 "/runtime/libc/include/stdio.h"
    vfscanf
# 82 "/runtime/libc/include/stdio.h"
           (
# 82 "/runtime/libc/include/stdio.h"
            FILE
# 82 "/runtime/libc/include/stdio.h"
                 *
# 82 "/runtime/libc/include/stdio.h"
                   restrict
# 82 "/runtime/libc/include/stdio.h"
                            stream
# 82 "/runtime/libc/include/stdio.h"
                                  ,
# 82 "/runtime/libc/include/stdio.h"
                                    const
# 82 "/runtime/libc/include/stdio.h"
                                          char
# 82 "/runtime/libc/include/stdio.h"
                                               *
# 82 "/runtime/libc/include/stdio.h"
                                                 restrict
# 82 "/runtime/libc/include/stdio.h"
                                                          fmt
# 82 "/runtime/libc/include/stdio.h"
                                                             ,
# 82 "/runtime/libc/include/stdio.h"
                                                               va_list
# 82 "/runtime/libc/include/stdio.h"
                                                                       arg
# 82 "/runtime/libc/include/stdio.h"
                                                                          )
# 82 "/runtime/libc/include/stdio.h"
                                                                           ;
# 84 "/runtime/libc/include/stdio.h"
int
# 84 "/runtime/libc/include/stdio.h"
    vprintf
# 84 "/runtime/libc/include/stdio.h"
           (
# 84 "/runtime/libc/include/stdio.h"
            const
# 84 "/runtime/libc/include/stdio.h"
                  char
# 84 "/runtime/libc/include/stdio.h"
                       *
# 84 "/runtime/libc/include/stdio.h"
                         restrict
# 84 "/runtime/libc/include/stdio.h"
                                  fmt
# 84 "/runtime/libc/include/stdio.h"
                                     ,
# 84 "/runtime/libc/include/stdio.h"
                                       va_list
# 84 "/runtime/libc/include/stdio.h"
                                               arg
# 84 "/runtime/libc/include/stdio.h"
                                                  )
# 84 "/runtime/libc/include/stdio.h"
                                                   ;
# 85 "/runtime/libc/include/stdio.h"
int
# 85 "/runtime/libc/include/stdio.h"
    vscanf
# 85 "/runtime/libc/include/stdio.h"
          (
# 85 "/runtime/libc/include/stdio.h"
           const
# 85 "/runtime/libc/include/stdio.h"
                 char
# 85 "/runtime/libc/include/stdio.h"
                      *
# 85 "/runtime/libc/include/stdio.h"
                        restrict
# 85 "/runtime/libc/include/stdio.h"
                                 fmt
# 85 "/runtime/libc/include/stdio.h"
                                    ,
# 85 "/runtime/libc/include/stdio.h"
                                      va_list
# 85 "/runtime/libc/include/stdio.h"
                                              arg
# 85 "/runtime/libc/include/stdio.h"
                                                 )
# 85 "/runtime/libc/include/stdio.h"
                                                  ;
# 87 "/runtime/libc/include/stdio.h"
int
# 87 "/runtime/libc/include/stdio.h"
    vsnprintf
# 87 "/runtime/libc/include/stdio.h"
             (
# 87 "/runtime/libc/include/stdio.h"
              char
# 87 "/runtime/libc/include/stdio.h"
                   *
# 87 "/runtime/libc/include/stdio.h"
                     restrict
# 87 "/runtime/libc/include/stdio.h"
                              s
# 87 "/runtime/libc/include/stdio.h"
                               ,
# 87 "/runtime/libc/include/stdio.h"
                                 size_t
# 87 "/runtime/libc/include/stdio.h"
                                        n
# 87 "/runtime/libc/include/stdio.h"
                                         ,
# 87 "/runtime/libc/include/stdio.h"
                                           const
# 87 "/runtime/libc/include/stdio.h"
                                                 char
# 87 "/runtime/libc/include/stdio.h"
                                                      *
# 87 "/runtime/libc/include/stdio.h"
                                                        restrict
# 87 "/runtime/libc/include/stdio.h"
                                                                 fmt
# 87 "/runtime/libc/include/stdio.h"
                                                                    ,
# 87 "/runtime/libc/include/stdio.h"
                                                                      va_list
# 87 "/runtime/libc/include/stdio.h"
                                                                              arg
# 87 "/runtime/libc/include/stdio.h"
                                                                                 )
# 87 "/runtime/libc/include/stdio.h"
                                                                                  ;
# 88 "/runtime/libc/include/stdio.h"
int
# 88 "/runtime/libc/include/stdio.h"
    vsprintf
# 88 "/runtime/libc/include/stdio.h"
            (
# 88 "/runtime/libc/include/stdio.h"
             char
# 88 "/runtime/libc/include/stdio.h"
                  *
# 88 "/runtime/libc/include/stdio.h"
                    restrict
# 88 "/runtime/libc/include/stdio.h"
                             s
# 88 "/runtime/libc/include/stdio.h"
                              ,
# 88 "/runtime/libc/include/stdio.h"
                                const
# 88 "/runtime/libc/include/stdio.h"
                                      char
# 88 "/runtime/libc/include/stdio.h"
                                           *
# 88 "/runtime/libc/include/stdio.h"
                                             restrict
# 88 "/runtime/libc/include/stdio.h"
                                                      fmt
# 88 "/runtime/libc/include/stdio.h"
                                                         ,
# 88 "/runtime/libc/include/stdio.h"
                                                           va_list
# 88 "/runtime/libc/include/stdio.h"
                                                                   arg
# 88 "/runtime/libc/include/stdio.h"
                                                                      )
# 88 "/runtime/libc/include/stdio.h"
                                                                       ;
# 89 "/runtime/libc/include/stdio.h"
int
# 89 "/runtime/libc/include/stdio.h"
    vsscanf
# 89 "/runtime/libc/include/stdio.h"
           (
# 89 "/runtime/libc/include/stdio.h"
            const
# 89 "/runtime/libc/include/stdio.h"
                  char
# 89 "/runtime/libc/include/stdio.h"
                       *
# 89 "/runtime/libc/include/stdio.h"
                         restrict
# 89 "/runtime/libc/include/stdio.h"
                                  s
# 89 "/runtime/libc/include/stdio.h"
                                   ,
# 89 "/runtime/libc/include/stdio.h"
                                     const
# 89 "/runtime/libc/include/stdio.h"
                                           char
# 89 "/runtime/libc/include/stdio.h"
                                                *
# 89 "/runtime/libc/include/stdio.h"
                                                  restrict
# 89 "/runtime/libc/include/stdio.h"
                                                           fmt
# 89 "/runtime/libc/include/stdio.h"
                                                              ,
# 89 "/runtime/libc/include/stdio.h"
                                                                va_list
# 89 "/runtime/libc/include/stdio.h"
                                                                        arg
# 89 "/runtime/libc/include/stdio.h"
                                                                           )
# 89 "/runtime/libc/include/stdio.h"
                                                                            ;
# 91 "/runtime/libc/include/stdio.h"
int
# 91 "/runtime/libc/include/stdio.h"
    fgetc
# 91 "/runtime/libc/include/stdio.h"
         (
# 91 "/runtime/libc/include/stdio.h"
          FILE
# 91 "/runtime/libc/include/stdio.h"
               *
# 91 "/runtime/libc/include/stdio.h"
                stream
# 91 "/runtime/libc/include/stdio.h"
                      )
# 91 "/runtime/libc/include/stdio.h"
                       ;
# 92 "/runtime/libc/include/stdio.h"
char
# 92 "/runtime/libc/include/stdio.h"
     *
# 92 "/runtime/libc/include/stdio.h"
      fgets
# 92 "/runtime/libc/include/stdio.h"
           (
# 92 "/runtime/libc/include/stdio.h"
            char
# 92 "/runtime/libc/include/stdio.h"
                 *
# 92 "/runtime/libc/include/stdio.h"
                   restrict
# 92 "/runtime/libc/include/stdio.h"
                            s
# 92 "/runtime/libc/include/stdio.h"
                             ,
# 92 "/runtime/libc/include/stdio.h"
                               int
# 92 "/runtime/libc/include/stdio.h"
                                   n
# 92 "/runtime/libc/include/stdio.h"
                                    ,
# 92 "/runtime/libc/include/stdio.h"
                                      FILE
# 92 "/runtime/libc/include/stdio.h"
                                           *
# 92 "/runtime/libc/include/stdio.h"
                                             restrict
# 92 "/runtime/libc/include/stdio.h"
                                                      stream
# 92 "/runtime/libc/include/stdio.h"
                                                            )
# 92 "/runtime/libc/include/stdio.h"
                                                             ;
# 94 "/runtime/libc/include/stdio.h"
int
# 94 "/runtime/libc/include/stdio.h"
    fputc
# 94 "/runtime/libc/include/stdio.h"
         (
# 94 "/runtime/libc/include/stdio.h"
          int
# 94 "/runtime/libc/include/stdio.h"
              c
# 94 "/runtime/libc/include/stdio.h"
               ,
# 94 "/runtime/libc/include/stdio.h"
                 FILE
# 94 "/runtime/libc/include/stdio.h"
                      *
# 94 "/runtime/libc/include/stdio.h"
                       stream
# 94 "/runtime/libc/include/stdio.h"
                             )
# 94 "/runtime/libc/include/stdio.h"
                              ;
# 95 "/runtime/libc/include/stdio.h"
int
# 95 "/runtime/libc/include/stdio.h"
    fputs
# 95 "/runtime/libc/include/stdio.h"
         (
# 95 "/runtime/libc/include/stdio.h"
          const
# 95 "/runtime/libc/include/stdio.h"
                char
# 95 "/runtime/libc/include/stdio.h"
                     *
# 95 "/runtime/libc/include/stdio.h"
                       restrict
# 95 "/runtime/libc/include/stdio.h"
                                s
# 95 "/runtime/libc/include/stdio.h"
                                 ,
# 95 "/runtime/libc/include/stdio.h"
                                   FILE
# 95 "/runtime/libc/include/stdio.h"
                                        *
# 95 "/runtime/libc/include/stdio.h"
                                          restrict
# 95 "/runtime/libc/include/stdio.h"
                                                   stream
# 95 "/runtime/libc/include/stdio.h"
                                                         )
# 95 "/runtime/libc/include/stdio.h"
                                                          ;
# 97 "/runtime/libc/include/stdio.h"
int
# 97 "/runtime/libc/include/stdio.h"
    getc
# 97 "/runtime/libc/include/stdio.h"
        (
# 97 "/runtime/libc/include/stdio.h"
         FILE
# 97 "/runtime/libc/include/stdio.h"
              *
# 97 "/runtime/libc/include/stdio.h"
               stream
# 97 "/runtime/libc/include/stdio.h"
                     )
# 97 "/runtime/libc/include/stdio.h"
                      ;
# 98 "/runtime/libc/include/stdio.h"
int
# 98 "/runtime/libc/include/stdio.h"
    getchar
# 98 "/runtime/libc/include/stdio.h"
           (
# 98 "/runtime/libc/include/stdio.h"
            void
# 98 "/runtime/libc/include/stdio.h"
                )
# 98 "/runtime/libc/include/stdio.h"
                 ;
# 100 "/runtime/libc/include/stdio.h"
int
# 100 "/runtime/libc/include/stdio.h"
    putc
# 100 "/runtime/libc/include/stdio.h"
        (
# 100 "/runtime/libc/include/stdio.h"
         int
# 100 "/runtime/libc/include/stdio.h"
             c
# 100 "/runtime/libc/include/stdio.h"
              ,
# 100 "/runtime/libc/include/stdio.h"
                FILE
# 100 "/runtime/libc/include/stdio.h"
                     *
# 100 "/runtime/libc/include/stdio.h"
                      stream
# 100 "/runtime/libc/include/stdio.h"
                            )
# 100 "/runtime/libc/include/stdio.h"
                             ;
# 101 "/runtime/libc/include/stdio.h"
int
# 101 "/runtime/libc/include/stdio.h"
    putchar
# 101 "/runtime/libc/include/stdio.h"
           (
# 101 "/runtime/libc/include/stdio.h"
            int
# 101 "/runtime/libc/include/stdio.h"
                c
# 101 "/runtime/libc/include/stdio.h"
                 )
# 101 "/runtime/libc/include/stdio.h"
                  ;
# 103 "/runtime/libc/include/stdio.h"
int
# 103 "/runtime/libc/include/stdio.h"
    puts
# 103 "/runtime/libc/include/stdio.h"
        (
# 103 "/runtime/libc/include/stdio.h"
         const
# 103 "/runtime/libc/include/stdio.h"
               char
# 103 "/runtime/libc/include/stdio.h"
                    *
# 103 "/runtime/libc/include/stdio.h"
                     s
# 103 "/runtime/libc/include/stdio.h"
                      )
# 103 "/runtime/libc/include/stdio.h"
                       ;
# 104 "/runtime/libc/include/stdio.h"
int
# 104 "/runtime/libc/include/stdio.h"
    ungetc
# 104 "/runtime/libc/include/stdio.h"
          (
# 104 "/runtime/libc/include/stdio.h"
           int
# 104 "/runtime/libc/include/stdio.h"
               c
# 104 "/runtime/libc/include/stdio.h"
                ,
# 104 "/runtime/libc/include/stdio.h"
                  FILE
# 104 "/runtime/libc/include/stdio.h"
                       *
# 104 "/runtime/libc/include/stdio.h"
                        stream
# 104 "/runtime/libc/include/stdio.h"
                              )
# 104 "/runtime/libc/include/stdio.h"
                               ;
# 106 "/runtime/libc/include/stdio.h"
size_t
# 106 "/runtime/libc/include/stdio.h"
       fread
# 106 "/runtime/libc/include/stdio.h"
            (
# 106 "/runtime/libc/include/stdio.h"
             void
# 106 "/runtime/libc/include/stdio.h"
                  *
# 106 "/runtime/libc/include/stdio.h"
                    restrict
# 106 "/runtime/libc/include/stdio.h"
                             p
# 106 "/runtime/libc/include/stdio.h"
                              ,
# 106 "/runtime/libc/include/stdio.h"
                                size_t
# 106 "/runtime/libc/include/stdio.h"
                                       size
# 106 "/runtime/libc/include/stdio.h"
                                           ,
# 106 "/runtime/libc/include/stdio.h"
                                             size_t
# 106 "/runtime/libc/include/stdio.h"
                                                    nmemb
# 106 "/runtime/libc/include/stdio.h"
                                                         ,
# 106 "/runtime/libc/include/stdio.h"
                                                           FILE
# 106 "/runtime/libc/include/stdio.h"
                                                                *
# 106 "/runtime/libc/include/stdio.h"
                                                                  restrict
# 106 "/runtime/libc/include/stdio.h"
                                                                           stream
# 106 "/runtime/libc/include/stdio.h"
                                                                                 )
# 106 "/runtime/libc/include/stdio.h"
                                                                                  ;
# 107 "/runtime/libc/include/stdio.h"
size_t
# 107 "/runtime/libc/include/stdio.h"
       fwrite
# 107 "/runtime/libc/include/stdio.h"
             (
# 107 "/runtime/libc/include/stdio.h"
              const
# 107 "/runtime/libc/include/stdio.h"
                    void
# 107 "/runtime/libc/include/stdio.h"
                         *
# 107 "/runtime/libc/include/stdio.h"
                           restrict
# 107 "/runtime/libc/include/stdio.h"
                                    p
# 107 "/runtime/libc/include/stdio.h"
                                     ,
# 107 "/runtime/libc/include/stdio.h"
                                       size_t
# 107 "/runtime/libc/include/stdio.h"
                                              size
# 107 "/runtime/libc/include/stdio.h"
                                                  ,
# 107 "/runtime/libc/include/stdio.h"
                                                    size_t
# 107 "/runtime/libc/include/stdio.h"
                                                           nmemb
# 107 "/runtime/libc/include/stdio.h"
                                                                ,
# 107 "/runtime/libc/include/stdio.h"
                                                                  FILE
# 107 "/runtime/libc/include/stdio.h"
                                                                       *
# 107 "/runtime/libc/include/stdio.h"
                                                                         restrict
# 107 "/runtime/libc/include/stdio.h"
                                                                                  stream
# 107 "/runtime/libc/include/stdio.h"
                                                                                        )
# 107 "/runtime/libc/include/stdio.h"
                                                                                         ;
# 109 "/runtime/libc/include/stdio.h"
int
# 109 "/runtime/libc/include/stdio.h"
    fgetpos
# 109 "/runtime/libc/include/stdio.h"
           (
# 109 "/runtime/libc/include/stdio.h"
            FILE
# 109 "/runtime/libc/include/stdio.h"
                 *
# 109 "/runtime/libc/include/stdio.h"
                   restrict
# 109 "/runtime/libc/include/stdio.h"
                            stream
# 109 "/runtime/libc/include/stdio.h"
                                  ,
# 109 "/runtime/libc/include/stdio.h"
                                    fpos_t
# 109 "/runtime/libc/include/stdio.h"
                                           *
# 109 "/runtime/libc/include/stdio.h"
                                             restrict
# 109 "/runtime/libc/include/stdio.h"
                                                      pos
# 109 "/runtime/libc/include/stdio.h"
                                                         )
# 109 "/runtime/libc/include/stdio.h"
                                                          ;
# 110 "/runtime/libc/include/stdio.h"
int
# 110 "/runtime/libc/include/stdio.h"
    fseek
# 110 "/runtime/libc/include/stdio.h"
         (
# 110 "/runtime/libc/include/stdio.h"
          FILE
# 110 "/runtime/libc/include/stdio.h"
               *
# 110 "/runtime/libc/include/stdio.h"
                stream
# 110 "/runtime/libc/include/stdio.h"
                      ,
# 110 "/runtime/libc/include/stdio.h"
                        long
# 110 "/runtime/libc/include/stdio.h"
                             int
# 110 "/runtime/libc/include/stdio.h"
                                 offset
# 110 "/runtime/libc/include/stdio.h"
                                       ,
# 110 "/runtime/libc/include/stdio.h"
                                         int
# 110 "/runtime/libc/include/stdio.h"
                                             whence
# 110 "/runtime/libc/include/stdio.h"
                                                   )
# 110 "/runtime/libc/include/stdio.h"
                                                    ;
# 111 "/runtime/libc/include/stdio.h"
int
# 111 "/runtime/libc/include/stdio.h"
    fsetpos
# 111 "/runtime/libc/include/stdio.h"
           (
# 111 "/runtime/libc/include/stdio.h"
            FILE
# 111 "/runtime/libc/include/stdio.h"
                 *
# 111 "/runtime/libc/include/stdio.h"
                  stream
# 111 "/runtime/libc/include/stdio.h"
                        ,
# 111 "/runtime/libc/include/stdio.h"
                          const
# 111 "/runtime/libc/include/stdio.h"
                                fpos_t
# 111 "/runtime/libc/include/stdio.h"
                                       *
# 111 "/runtime/libc/include/stdio.h"
                                        pos
# 111 "/runtime/libc/include/stdio.h"
                                           )
# 111 "/runtime/libc/include/stdio.h"
                                            ;
# 112 "/runtime/libc/include/stdio.h"
long
# 112 "/runtime/libc/include/stdio.h"
     int
# 112 "/runtime/libc/include/stdio.h"
         ftell
# 112 "/runtime/libc/include/stdio.h"
              (
# 112 "/runtime/libc/include/stdio.h"
               FILE
# 112 "/runtime/libc/include/stdio.h"
                    *
# 112 "/runtime/libc/include/stdio.h"
                     stream
# 112 "/runtime/libc/include/stdio.h"
                           )
# 112 "/runtime/libc/include/stdio.h"
                            ;
# 113 "/runtime/libc/include/stdio.h"
void
# 113 "/runtime/libc/include/stdio.h"
     rewind
# 113 "/runtime/libc/include/stdio.h"
           (
# 113 "/runtime/libc/include/stdio.h"
            FILE
# 113 "/runtime/libc/include/stdio.h"
                 *
# 113 "/runtime/libc/include/stdio.h"
                  stream
# 113 "/runtime/libc/include/stdio.h"
                        )
# 113 "/runtime/libc/include/stdio.h"
                         ;
# 114 "/runtime/libc/include/stdio.h"
void
# 114 "/runtime/libc/include/stdio.h"
     clearerr
# 114 "/runtime/libc/include/stdio.h"
             (
# 114 "/runtime/libc/include/stdio.h"
              FILE
# 114 "/runtime/libc/include/stdio.h"
                   *
# 114 "/runtime/libc/include/stdio.h"
                    stream
# 114 "/runtime/libc/include/stdio.h"
                          )
# 114 "/runtime/libc/include/stdio.h"
                           ;
# 115 "/runtime/libc/include/stdio.h"
int
# 115 "/runtime/libc/include/stdio.h"
    feof
# 115 "/runtime/libc/include/stdio.h"
        (
# 115 "/runtime/libc/include/stdio.h"
         FILE
# 115 "/runtime/libc/include/stdio.h"
              *
# 115 "/runtime/libc/include/stdio.h"
               stream
# 115 "/runtime/libc/include/stdio.h"
                     )
# 115 "/runtime/libc/include/stdio.h"
                      ;
# 116 "/runtime/libc/include/stdio.h"
int
# 116 "/runtime/libc/include/stdio.h"
    ferror
# 116 "/runtime/libc/include/stdio.h"
          (
# 116 "/runtime/libc/include/stdio.h"
           FILE
# 116 "/runtime/libc/include/stdio.h"
                *
# 116 "/runtime/libc/include/stdio.h"
                 stream
# 116 "/runtime/libc/include/stdio.h"
                       )
# 116 "/runtime/libc/include/stdio.h"
                        ;
# 117 "/runtime/libc/include/stdio.h"
void
# 117 "/runtime/libc/include/stdio.h"
     perror
# 117 "/runtime/libc/include/stdio.h"
           (
# 117 "/runtime/libc/include/stdio.h"
            const
# 117 "/runtime/libc/include/stdio.h"
                  char
# 117 "/runtime/libc/include/stdio.h"
                       *
# 117 "/runtime/libc/include/stdio.h"
                        s
# 117 "/runtime/libc/include/stdio.h"
                         )
# 117 "/runtime/libc/include/stdio.h"
                          ;
# 4 "/runtime/libc/include/posix/stdio.h"
FILE
# 4 "/runtime/libc/include/posix/stdio.h"
     *
# 4 "/runtime/libc/include/posix/stdio.h"
      fdopen
# 4 "/runtime/libc/include/posix/stdio.h"
            (
# 4 "/runtime/libc/include/posix/stdio.h"
             int
# 4 "/runtime/libc/include/posix/stdio.h"
                ,
# 4 "/runtime/libc/include/posix/stdio.h"
                  const
# 4 "/runtime/libc/include/posix/stdio.h"
                        char
# 4 "/runtime/libc/include/posix/stdio.h"
                             *
# 4 "/runtime/libc/include/posix/stdio.h"
                              )
# 4 "/runtime/libc/include/posix/stdio.h"
                               ;
# 6 "/runtime/libc/include/posix/stdio.h"
int
# 6 "/runtime/libc/include/posix/stdio.h"
    fileno
# 6 "/runtime/libc/include/posix/stdio.h"
           (
# 6 "/runtime/libc/include/posix/stdio.h"
            FILE
# 6 "/runtime/libc/include/posix/stdio.h"
                *
# 6 "/runtime/libc/include/posix/stdio.h"
                 )
# 6 "/runtime/libc/include/posix/stdio.h"
                  ;
# 2 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-c11-line/build/library-focused-3/sources/stdio.c"
int
# 2 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-c11-line/build/library-focused-3/sources/stdio.c"
    main
# 2 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-c11-line/build/library-focused-3/sources/stdio.c"
        (
# 2 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-c11-line/build/library-focused-3/sources/stdio.c"
         void
# 2 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-c11-line/build/library-focused-3/sources/stdio.c"
             )
# 2 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-c11-line/build/library-focused-3/sources/stdio.c"
              {
# 2 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-c11-line/build/library-focused-3/sources/stdio.c"
               return
# 2 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-c11-line/build/library-focused-3/sources/stdio.c"
                      printf
# 2 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-c11-line/build/library-focused-3/sources/stdio.c"
                            (
# 2 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-c11-line/build/library-focused-3/sources/stdio.c"
                             "id %d\n"
# 2 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-c11-line/build/library-focused-3/sources/stdio.c"
                                      ,
# 2 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-c11-line/build/library-focused-3/sources/stdio.c"
                                       7
# 2 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-c11-line/build/library-focused-3/sources/stdio.c"
                                        )
# 2 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-c11-line/build/library-focused-3/sources/stdio.c"
                                         !=
# 2 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-c11-line/build/library-focused-3/sources/stdio.c"
                                           5
# 2 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-c11-line/build/library-focused-3/sources/stdio.c"
                                            ;
# 2 "/data/home/kirancodes/Documents/code/ship-your-tcc/.claude/worktrees/sol-c11-line/build/library-focused-3/sources/stdio.c"
                                             }
