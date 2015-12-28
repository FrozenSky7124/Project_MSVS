SHELL
=====

$# 参数的个数

$i 第 i 个参数

$* 所有参数

`expr $a + $b`  算数命令

**字符集转换**

```c
	#include <iconv.h>
	iconv_t iconv_open(const char *tocode, const char *fromcode);

	size_t iconv(iconv_t cd, char **inbuf, size_t *inbytesleft,
		char **outbuf, size_t *outbytesleft);

	iconv_close(iconv_t cd);
```