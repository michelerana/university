#ifndef SRC_CF_UTILITIES_H_
#define SRC_CF_UTILITIES_H_

#include <stdbool.h>

/**
 * Costante contenente il numero di comuni esistenti.
 */
#define N_CITIES 8092

/**
 * Costante del carattere di fine stringa
 */
#define EOS '\0'
/**
 * Lunghezza minima della stringa del cognome
 */
#define SURNAME_MIN 2
/**
 * Quantita' minima di lettere maiuscole
 */
#define CAP_LET_MIN 2
/**
 * Lunghezza minima della stringa del luogo
 */
#define PLACE_MIN 2
/**
 * Lunghezza della stringa della data in formato GG/MM/AAAA
 */
#define DATE_LEN 10

/**
 * Costanti di mesi contenenti il valore numerico di febbraio
 * e dei mesi che hanno 30 giorni.
 */
enum { FEB = 2, /**< 2 *///!< FEB
	   APR = 4, /**< 4 */   //!< APR
	   JUN = 6, /**< 6 */   //!< JUN
	   SEP = 9, /**< 9 */   //!< SEP
	   NOV = 11 /**< 11 */  //!< NOV
};

/**
 * @struct city
 * @brief Struttura contenente il codice catastale e il nome di un comune
 * @var city::code
 * Membro contenente il codice castale del comune
 * @var city::name
 * Membro contenente il nome del comune
 */
typedef struct {
		char code[5];
		char name[36];
} city;

/**
 * Stringa di mesi codificati nel formato codice fiscale.
 */
static const char months_coded[] = "ABCDEHLMPRST";

/**
 * Verifica se il cognome contiene solo caratteri maiuscoli, spazi e apici.
 * @param surname Cognome
 * @return Valore di verita'
 */
bool is_surname(const char surname[]);

/**
 * Verifica se il nome contiene solo caratteri maiuscoli, spazi e apici.
 * @param name Nome
 * @return Valore di verita'
 */
bool is_name(const char name[]);

/**
 * Verifica se il sesso della persona e' 'M' oppure 'F'
 * @param sex Sesso
 * @return Valore di verita'
 */
bool is_sex(const char sex);

/**
 * Verifica se la data e' nel formato GG/MM/AAAA
 * @param date_org Data
 * @return Valore di verita'
 */
bool is_date(const char date_org[]);

/**
 * Verifica se la citta' contiene solo caratteri maiuscoli, spazi, apici e trattini.
 * @param place Citta'
 * @return Valore di verita'
 */
bool is_place(const char place[]);

/**
 * Verifica se la lettera e' una vocale
 * @param letter Lettera
 * @return Valore di verita'
 */
bool is_vocal(const char letter);

/**
 * Calcola il valore del codice alfanumerico di un carattere del codice fiscale
 * @param code Carattere del codice fiscale
 * @param idx Posizione del carattere del codice fiscale
 * @return Valore del codice alfanumerico
 * @pre code dev'essere un carattere maiuscolo o una cifra
 * @pre idx dev'essere valore compreso nell'intervallo 1 <= idx <= 15
 */
int generate_acode(const char code, const int idx);

/**
 * Verifica se il cognome e' correttamente codificato nel formato codice fiscale.
 * @param coded_surname Cognome
 * @return Valore di verita'
 */
bool is_coded_surname(const char coded_surname[]);

/**
 * Verifica se il nome e' correttamente codificato nel formato codice fiscale.
 * @param coded_name Nome
 * @return Valore di verita'
 */
bool is_coded_name(const char coded_name[]);

/**
 * Verifica se la data e' correttamente codificata nel formato codice fiscale.
 * @param coded_date Data
 * @return Valore di verita'
 */
bool is_coded_date(const char coded_date[]);

/**
 * Verifica se la citta' e' correttamente codificata nel formato codice fiscale.
 * @param coded_place Luogo di nascita in formato codice fiscale
 * @return Valore di verita'.
 */
bool is_coded_place(const char coded_place[]);

/**
 * Verifica se il codice di controllo e' correttamente codificato nel formato codice fiscale.
 * @param ctrl_code Codice di controllo
 * @return Valore di verita'
 */
bool is_ctrl_code(const char ctrl_code);

#endif /* SRC_CF_UTILITIES_H_ */
