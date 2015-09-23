#ifndef CF_GENERATOR_H_
#define CF_GENERATOR_H_

#include <stdbool.h>

/**
 * Lunghezza massima del vettore di vocali del codice fiscale
 */
#define VOW_MAX 3
/**
 * Lunghezza massima del vettore di consonanti del codice fiscale
 */
#define CON_MAX 3
/**
 * Lunghezza massima del vettore contenente i caratteri del codice fiscale
 * (cognome o nome)
 */
#define COD_MAX 3

/**
 * Codifica il cognome nel formato per codice fiscale.
 * @param org_surname	Cognome.
 * @param coded_surname	Stringa da 3 caratteri che conterra' la codifica.
 * @pre surname puo' contenere solo caratteri maiuscoli, spazi, apici.
 * @pre surname deve essere formato da almeno due caratteri.
 * @post coded_surname deve contenere 3 lettere maiuscole.
 */
void surname2code(const char org_surname[], char coded_surname[]);

/**
 * Codifica il nome nel formato per codice fiscale.
 * @param name		 Nome.
 * @param coded_name Stringa da 3 caratteri che conterra' la codifica.
 * @pre name puo' contenere solo caratteri maiuscoli, spazi, apici.
 * @pre name deve essere formato da almeno due caratteri.
 * @post coded_name deve contenere 3 lettere maiuscole.
 */
void name2code(const char name[], char coded_name[]);

/**
 * Codifica la data di nascita nel formato per codice fiscale.
 * @param date 		 Data di nascita nel formato GG/MM/AAAA.
 * @param sex 		 Sesso.
 * @param coded_date Stringa di 5 caratteri che conterra' la codifica.
 * @pre date puo' contenere solo numeri e 2 caratteri separatori '/'
 * @pre date deve essere fornita nel formato GG/MM/AAAA con 0 < GG <= 31 ; 0 < MM <= 12; 1900 < AAAA <= 2015
 * @pre sex puo' essere 'M' o 'F'
 * @post coded_date deve essere una stringa di 5 caratteri
 * @post deve esser composta da (in ordine): 2 cifre, 1 lettera, 2 cifre
 */
void date2code(const char date[], const char sex, char coded_date[]);

/**
 * Calcola il codice di controllo di un codice fiscale (l'ultimo carattere).
 * @param coded_surname 	Cognome codificato (stringa da 3 caratteri).
 * @param coded_name 		Nome codificato (stringa da 3 caratteri).
 * @param coded_date 		Data di nascita codificata (stringa da 5 caratteri).
 * @param coded_birth_place Codice catastale del luogo di nascita (striga da 4 caratteri).
 * @return Carattere di controllo.
 * @pre coded_surname deve essere una stringa di 3 caratteri maiuscoli.
 * @pre coded_name deve essere una stringa di 3 caratteri maiuscoli.
 * @pre coded_date deve essere una stringa di 5 caratteri (2 numeri, 1 lettera, 2 numeri).
 * @pre birth_place_code deve essere una stringa di 4 caratteri (1 lettera, 3 numeri).
 * @post il carattere di controllo deve essere una lettera maiuscola.
 */
char cf_ctrl_code(const char coded_surname[], const char coded_name[],
     const char coded_date[], const char coded_birth_place[]);

/**
 * Calcola il codice fiscale di una persona.
 * @param surname Cognome.
 * @param name Nome.
 * @param date Data di nascita.
 * @param birth_place_code Codice catastale del luogo di nascita.
 * @param sex Sesso.
 * @param cf Codice fiscale (inizializzato a stringa vuota "").
 * @pre surname puo' contenere solo caratteri maiuscoli, spazi, apici.
 * @pre name puo' contenere solo caratteri maiuscoli, spazi, apici.
 * @pre date puo' contenere solo numeri e 2 caratteri separatori '/'
 * @pre date deve essere fornita nel formato GG/MM/AAAA con 0 < GG <= 31 ; 0 < MM <= 12; 1900 < AAAA <= 2015
 * @pre birth_place_code codice catastale del luogo di nascita composto da una lettera e tre cifre
 * @pre sex puo' essere 'M' o 'F'
 * @post cf deve essere una stringa composta da (in ordine): 6 lettere, 2 numeri, 1 lettera, 2 numeri, 1 lettera, 3 numeri, 1 lettera
 */
void cf_generator(const char surname[], const char name[], const char date[],
     const char birth_place_code[], const char sex, char cf[]);

/**
 * Calcola il codice catastale di una citta' se esiste
 * @param place Citta'
 * @param coded_place Codice catastale (inizializzato a stringa vuota "").
 * @return Valore di verita' sull'esistenza della citta'
 * @pre place puo' contenere solo caratteri maiuscoli, spazi, apici, trattini
 * @post coded_place deve essere una stringa di 4 caratteri (1 lettera, 3 numeri).
 */
bool get_coded_place(const char place[], char coded_place[]);

#endif /* CF_GENERATOR_H_ */
