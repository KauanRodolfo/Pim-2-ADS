/* ==========================================================
   util.h
   Funcoes de apoio para leitura do teclado.
   ========================================================== */
#ifndef UTIL_H
#define UTIL_H

/* Remove o '\n' do fim do texto lido com fgets. */
void tratar_texto(char *variavel);
/* Descarta o que sobrou no buffer do teclado. */
void limpar_buffer(void);

#endif
