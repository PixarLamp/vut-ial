
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, 2014-2018
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {
	for(int i = 0; i < HTSIZE; i++){
		(*ptrht)[i] = NULL; //nastavenie všetkých položiek v tabuľke na hodnotu NULL
	}
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není,
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {
	if(ptrht == NULL){
		return NULL;
	}
	int index = hashCode(key); //zistenie indexu zadanej položky v tabuľke
	tHTItem *item = (*ptrht)[index];

	for (; item; item = item->ptrnext){ //prechádzanie lineárneho zoznamu na danom indexe
		if (item->key == key){
			return item; //položka bola nájdená
		}
	}	
	return NULL; //položka nebola nájdená
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {
	if(ptrht == NULL){
		return;
	}
	int index = hashCode(key); //zistenie indexu zadanej položky v tabuľke
	tHTItem *item = htSearch(ptrht, key);

	if (item != NULL){
		item->data = data; //položka sa už nachádza v tabuľke, jej data sa aktualizujú a funkcia tu končí
		return;
	}
	else{
		item = (tHTItem *)malloc(sizeof(tHTItem)); //alokácia pamäte pre novú položku
		if(!item){
			return; //alokácia zlihala
		}
		else{
			item->key = key; //vpísanie kľúču a dát
			item->data = data;

			if((*ptrht)[index] == NULL){ //položka je prvá na danom indexe, bude prvá v danom lineárnom zozname
				(*ptrht)[index] = item;
				item->ptrnext = NULL;
			}
			else{
				item->ptrnext = (*ptrht)[index]; //vloženie položky na začiatok zoznamu a nastavenie jej ukazovateľa
				(*ptrht)[index] = item;
			}
		}
	}
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {
	if(ptrht == NULL){
		return NULL;
	}
	tHTItem *item = htSearch(ptrht, key);
	if(item != NULL){
		return &(item->data); //položka bola nájdená, vracia sa jej hodnota
	}
	else{
		return NULL; //položka nebola nájdená
	}
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {
	if(ptrht == NULL){
		return;
	}
	int index = hashCode(key); //zistenie indexu na ktorom sa nachádza položka
	tHTItem *item = (*ptrht)[index]; //položka ktorá sa má vymazať
	tHTItem *prev_item = NULL;
	tHTItem *next_item = NULL;

	for (; item; item = item->ptrnext){ //hľadanie položky v lineárnom zozname na danom indexe
		if (item->key == key){
			next_item = item->ptrnext; //prenastavenie ukazovateľa
			free(item); //mazanie položky a uvoľnenie pamäte
			if (!prev_item)
			{
				(*ptrht)[index] = next_item;
				return;
			}
			prev_item->ptrnext = next_item;
			return;
		}
		prev_item = item;
	}
	return;
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {
	if(ptrht == NULL){
		return;
	}
	for(int index = 0; index < HTSIZE; index++){ //prechádza každý index poľa
		tHTItem *item = (*ptrht)[index];
		for (; item; item = item->ptrnext){ //prechádza každú položku lineárneho zoznamu na danom indexe
			(*ptrht)[index] = item->ptrnext; //nastavenie nového ukazovateľa
			free(item); //vymazanie položky
		}
		(*ptrht)[index] = NULL; //nastavenie tabuľky do stavu po inicializácii
	}	
}
