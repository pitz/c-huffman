#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stArvore {
	struct stArvore *esq;
	struct stArvore *dir;
	int    frequencia;
	char   letra;
} *arvore;
 
struct stArvore parvores[256];
arvore arvores[255];
arvore *ListaArvores = arvores - 1;
int  n_arvores = 0;
int  final_fila = 1;
char *cTabelaASCII[128];
char texto[1000];
	
arvore fCriaArvore(int frequencia, char c)
{
	arvore novaArvore      = parvores + n_arvores++;
	novaArvore->letra      = c;
	novaArvore->frequencia = frequencia;
	
	return novaArvore;
}

arvore fCriaEstruturaArvores(arvore Esq, arvore Dir)
{
	arvore novaArvore      = parvores + n_arvores++;
	novaArvore->esq        = Esq;
	novaArvore->dir        = Dir;
	novaArvore->frequencia = Esq->frequencia + Dir->frequencia;
	return novaArvore;
}

void fOrdenarArvores(arvore ArvoreRaiz)
{
	int i;
	int j;
	ListaArvores[final_fila++] = ArvoreRaiz;
	arvore ArvoreTmp;
			
	if (final_fila > 2) {
		for (i=final_fila-1; i>0; i--) {
	        for (j=1; j < i ; j++) {		    		
	        	if (ListaArvores[j]->frequencia > ListaArvores[i]->frequencia) {
	                ArvoreTmp       = ListaArvores[j];
	                ListaArvores[j] = ListaArvores[i];
	                ListaArvores[i] = ArvoreTmp;
	            }
	        }
	    }
	}
}

arvore fRetornaMenorFrequencia()
{
	int i;
	int l;
	arvore n = ListaArvores[i = 1];
 
	if (final_fila < 2) return 0;
	
	final_fila--;
	while ((l = i * 2) < final_fila) {
		if (l + 1 < final_fila && ListaArvores[l + 1]->frequencia < ListaArvores[l]->frequencia) l++;
		
		ListaArvores[i] = ListaArvores[l];
		i = l;
	}
	
	ListaArvores[i] = ListaArvores[final_fila];
	return n;
}

void fSubstituiCodigo(arvore Folha, char *s, int tamanho)
{
	static char *codigo = texto;
	
	if (Folha->letra) {
		s[tamanho] = 0;
		strcpy(codigo, s);			    
		cTabelaASCII[Folha->letra] = codigo;
		codigo += tamanho + 1;
		return;
	}
	
	s[tamanho] = '0'; fSubstituiCodigo(Folha->esq, s, tamanho + 1);
	s[tamanho] = '1'; fSubstituiCodigo(Folha->dir, s, tamanho + 1);
}

void init(const char *s)
{
	int iPosicao;
	int frequencia[128] = {0};
	char c[16];
	arvore ArvoreNova;
	
	///-------------------------------------------------------------------------------------///
	/// A tabela ASCII contém 128 números atribuídos aos caracteres correspondentes. 		///
	/// O ASCII  permite  que  os  computadores armazenem e troquem dados com outros		///
	/// computadores e programas.															///
	/// Fonte: 2015 Microsoft.																///
	/// Sabendo disso, a maneira mais facil (nao a melhor), de medir a freq. é 				///
	/// utilizar um array de 128 caracteres, convertendo o valor do Caracter para Decimal.	///
	/// Eduardo Pitz.																		///
	///-------------------------------------------------------------------------------------///
	
	while (*s) { frequencia[(int)*s++]++; }
	
	for (iPosicao=0;iPosicao<=127;iPosicao++) {
		if (frequencia[iPosicao]) {
			ArvoreNova = fCriaArvore(frequencia[iPosicao], iPosicao);
			fOrdenarArvores(ArvoreNova); 
		}
	}

	while (final_fila > 2) {
		ArvoreNova = fCriaEstruturaArvores(fRetornaMenorFrequencia(), fRetornaMenorFrequencia());
		fOrdenarArvores(ArvoreNova);
	}

	fSubstituiCodigo(ListaArvores[1], c, 0);
}

char fCompressao(const char *textoDescomprimido, char *textoComprimido)
{	
	while (*textoDescomprimido) {
		strcpy(textoComprimido, cTabelaASCII[*textoDescomprimido]);		
		textoComprimido += strlen(cTabelaASCII[*textoDescomprimido++]);
	}
}

void fDescompressao(const char *texto)
{
	arvore ArvoreTmp = ListaArvores[1];
	
	while (*texto) {
		if (*texto++ == '0') ArvoreTmp = ArvoreTmp->esq;
		else 				 ArvoreTmp = ArvoreTmp->dir;
 		
		if (ArvoreTmp->letra) {
			putchar(ArvoreTmp->letra);
			ArvoreTmp = ListaArvores[1];
		} 
	}
}
 
int main(void)
{
	int i;
	const char *str = "Joinville";
	char texto[1024];
	init(str);
	
	/*	for (i = 0; i < 128; i++)
		if (cTabelaASCII[i]) printf("'%c': %s\n", i, cTabelaASCII[i]); */
	
	fCompressao(str, texto); printf("Comprimido: %s\n", texto);
	printf("Descomprimido: "); fDescompressao(texto);
 
	return 0;
}