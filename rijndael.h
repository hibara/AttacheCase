#ifndef _rijndael_C_
#define _rijndael_C_

#ifdef __cplusplus
extern "C" {
#endif

void gentables(void);
void gkey(int nb, int nk, char *key);
void rijndael_encrypt(char *buff);
void rijndael_decrypt(char *buff);


#ifdef __cplusplus
}
#endif

#endif /* _rijndael_C_ */

