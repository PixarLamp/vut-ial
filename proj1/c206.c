
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2020
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**   d   DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**   d   DLInsertFirst ... vložení prvku na začátek seznamu,
**   d   DLInsertLast .... vložení prvku na konec seznamu,
**   d   DLFirst ......... nastavení aktivity na první prvek,
**   d   DLLast .......... nastavení aktivity na poslední prvek,
**   d   DLCopyFirst ..... vrací hodnotu prvního prvku,
**   d   DLCopyLast ...... vrací hodnotu posledního prvku,
**   d   DLDeleteFirst ... zruší první prvek seznamu,
**   d   DLDeleteLast .... zruší poslední prvek seznamu,
**   d   DLPostDelete .... ruší prvek za aktivním prvkem,
**   d   DLPreDelete ..... ruší prvek před aktivním prvkem,
**   d   DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**   d   DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**   d   DLCopy .......... vrací hodnotu aktivního prvku,
**   d   DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**   d   DLPred .......... posune aktivitu na předchozí prvek seznamu,
**   d   DLSucc .......... posune aktivitu na další prvek seznamu,
**   d   DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
   L->First = NULL; //nastavenie ukazovate+lov na hodnotu NULL
   L->Act = NULL;
   L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
	tDLElemPtr tDLElem = L->First; 
    while(L->First != NULL){ //kým nie je zoznam prázdny
        L->First = L->First->rptr; //1. prvkom v zozname sa stáva 2. prvok v poradí
        free(tDLElem); //maže sa predchádzajúci 1. prvok zoznamu
        tDLElem = L->First;
    }
    L->First = L->Act = L->Last = NULL; //uvedenie zoznamu do stavu po inicializácii
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	tDLElemPtr tDLElem = (tDLElemPtr) malloc(sizeof(struct tDLElem)); //alokácia pamäte pre nový prvok
    if(!tDLElem){ //kontrola alokácie pamäte
        DLError();
        return;
    }

    tDLElem->data = val; //nastavenie paramentrov nového prvku
    tDLElem->rptr = L->First;
    tDLElem->lptr = NULL;
    
    if(L->First == NULL){
        L->Last = tDLElem; //vkladaný prvok je jediným prvkom
    }
    else{
        L->First->lptr = tDLElem; //nastavenie ľavého ukazovateľa 1. prvku na nový prvok
    }
    L->First = tDLElem; //nový prvok je 1.
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 	
	tDLElemPtr tDLElem = (tDLElemPtr) malloc(sizeof(struct tDLElem)); //alokácia pamäte pre nový prvok
    if(!tDLElem){ //kontrola alokácie pamäti
        DLError();
        return;
    }

    tDLElem->data = val; //nastavenie paramentrov nového prvku
    tDLElem->rptr = NULL;
    tDLElem->lptr = L->Last;

    if(L->Last == NULL){
        L->First = tDLElem; //vkladaný prvok je jediným prvkom v zozname
    }
    else{
        L->Last->rptr = tDLElem; //nastavenie pravého ukazovateľa posledného prvku na nový posledný prvok
    }
    L->Last = tDLElem; //nový prvok je posledný
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->First; //1. prvok sa stáva aktívny
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->Last; //posledný prvok je aktívny
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    if(L->First == NULL){ //ošetrenie chyby pri prázdnom zozname
        DLError();
        return;
    }
	*val = L->First->data; //nastavenie návratovej hodnoty
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	if(L->First == NULL){ //ošetrenie chyby pri prázdnom zozname
        DLError();
        return;
    }
	*val = L->Last->data; //nastavenie návratovej hodnoty
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	if(L->First == NULL){ //zoznam prázdny, funkcia nič nemení
        return;
    }
	if(L->First == L->Act){ //1. prvok je aktívny prvok, zoznam stráca aktivitu
        L->Act = NULL;
    }
    tDLElemPtr tDLElem = L->First;
	L->First = tDLElem->rptr; //2. prvok bude 1. v poradí
    L->First->lptr = NULL;
    free(tDLElem); //mazanie pôvodného 1. prvku
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L.
** Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/ 
	if(L->First == NULL){ //zoznam prázdny, nič sa nezmení
        return;
    }
	if(L->Last == L->Act){ //posledný prvok je aktívny, zoznam stráca aktivitu
        L->Act = NULL;
    }
	tDLElemPtr tDLElem = L->Last;
	L->Last = tDLElem->lptr; //predposledný prvok sa stane posledným
    L->Last->rptr = NULL;
    free(tDLElem); //mazanie pôvodného posledného prvku
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
	if(L->Act == NULL || L->Last == L->Act){ 
        return; //zoznam nie je aktívny/posledný prvok je aktívny, nič sa nemení
    }
	tDLElemPtr tDLElem = L->Act->rptr;
    L->Act->rptr = tDLElem->rptr; //pravý ukazovateľ aktívneho prvku na praví ukazovateľ prvku, ktorý chcem vymazať
    if(tDLElem != L->Last){
        tDLElem->rptr->lptr = L->Act; //mazaný prvok nie je posledným prvkom, treba prenastaviť ľavý ukazovateľ prvku za mazaným prvkom
    }
    else{
        L->Last = L->Act; //mazaný prvok je posledným prvkom v zozname, aktívny prvok sa stáva aj posledným
    }
    free(tDLElem); //mazanie prvku
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
	if(L->Act == NULL || L->First == L->Act){
        return; //zoznam nie je aktívny/1. prvok je aktívny
    }
	tDLElemPtr tDLElem = L->Act->lptr;
    L->Act->lptr = tDLElem->lptr; //prenastavím ľavý ukazovateľ aktívneho prvku na ľavý ukazovateľ mazaného prvku
    if(tDLElem != L->First){
        tDLElem->lptr->rptr = L->Act; //mazaný prvok nie je 1. prvkom zoznamu, treba prenastaviť pravý ukazovateľ prvku pred mazaným prvkom
    }
    else{
        L->First = L->Act; //mazaný prvok je 1. v zozname, aktívny prvok sa stáva aj 1. prvkom zoznamu		
    }
    free(tDLElem);
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if(L->Act == NULL){
        return; //zoznam nie je aktívny, funkcia končí bez zmeny
    }
	tDLElemPtr tDLElem = (tDLElemPtr) malloc(sizeof(struct tDLElem)); //alokácia pamäte pre nový prvok
    if (!tDLElem){ //kontrola alokácie pamäte
        DLError(); 
        return;
    }
    tDLElem->data = val; //nastavenie hodnoty nového prvku
    tDLElem->rptr = L->Act->rptr; //pravým ukazovateľom nového prvku je pravý ukazovateľ aktívneho prvku
    tDLElem->lptr = L->Act; //ľavým ukazovateľom nového prvku sa stáva ukazovateľ na aktívny prvok
    L->Act->rptr = tDLElem; //pravý ukazovateľ aktívneho prvku bude ukazovať na novovložený prvok
    tDLElem = tDLElem->rptr;
    tDLElem->lptr = L->Act->rptr; //ľavý ukazovateľ prvku nasledujúceho po novovloženom prvku sa prepíše na ukazovateľ nového prvku
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
   if(L->Act == NULL){
        return; //zoznam nie je aktívny, funkcia končí bez zmeny
    }
	tDLElemPtr tDLElem = (tDLElemPtr) malloc(sizeof(struct tDLElem)); //alokácia pamäte pre nový prvok
    if(!tDLElem){ //kontrola alokácie mapäte
        DLError();
        return;
    }
    tDLElem->data = val; //nastavenie hodnoty nového prvku
    tDLElem->rptr = L->Act; //pravým ukazovateľom nového prvku sa stáva ukazovateľ na aktívny prvok
    tDLElem->lptr = L->Act->lptr; //ľavým ukazovateľom nového prvku je ľavý ukazovateľ aktívneho prvku
    L->Act->lptr = tDLElem; //ľavý ukazovateľ aktívneho prvku sa prepíše na ukazovateľ na nový prvok
    tDLElem = tDLElem->lptr;
    tDLElem->rptr = L->Act->lptr; //pravým ukazovateľom prvku pred vkladaným prvkom sa stáva vkladaný prvok

}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
	if(L->Act == NULL){
        DLError(); //zoznam nie je aktívny, ošetrenie chyby
        return;
    }	
	*val = L->Act->data; //zápis hodnoty aktívneho prvku
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
    if(L->Act == NULL){
        return; //zoznam nie je aktívny
    }
	L->Act->data = val; //prepis obsahu aktívneho prvku
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	if(L->Act == NULL){
        return; //zoznam nie je aktívny
    }
    if(L->Act == L->Last){
        L->Act = NULL; //aktivita je na poslednom prvku, zoznam stráca aktivitu
        return;
    }
	L->Act = L->Act->rptr; //aktivita sa presúva na nasledujúci prvok
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	if(L->Act == NULL){
        return; //zoznam nie je aktívny
    }
    if(L->Act == L->First){
        L->Act = NULL; //aktivita je na 1. prvku, zoznam stráca aktivitu
        return;
    }
	L->Act = L->Act->lptr; //aktivita sa presúva na predchádzajúci prvok
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
     return L->Act != NULL;
}

/* Konec c206.c*/
