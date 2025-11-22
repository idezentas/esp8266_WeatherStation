/**
 * @file Translations.cpp
 * @authors rob040@users.github.com and idezentas
 * @brief Provides language translation support for messages in the application.
 *
 * This module manages translation strings for multiple languages, allowing
 * retrieval of translated messages by message ID and language. It stores
 * translation strings and language metadata in PROGMEM for memory efficiency
 * on embedded platforms. The current language can be set and queried, and
 * translation strings can be accessed as C-strings or Arduino String objects.
 *
 * Features:
 * - Stores translation strings for multiple languages in PROGMEM.
 * - Provides functions to get language codes and names from language enums.
 * - Allows retrieval of translated messages by message ID and language.
 * - Supports setting and getting the current application language.
 * - Includes temporary buffer management for reading strings from PROGMEM.
 *
 * Usage:
 * - Use setCurrentLanguageId() to change the current language.
 * - Use getTranslation() or getTranslationStr() to retrieve translated messages.
 * - Use getLanguageCode() and getLanguageName() for language metadata.
 *
 * @copyright Copyright (c) 2025
 * @license MIT License (see LICENSE.txt for details)
 */
#include <Arduino.h>
#include "pgmspace.h"
#include "Translations.h"

/* Global variables */
static lang_t currentLanguage = LANG_EN; // default language is English

// Translation strings in PROGMEM
#define SP(lang, msg, str) const char tr_msg_##msg##_##lang[] PROGMEM = str;
#define TR(msg, l1, l2, l3, l4) \
  SP(EN, msg, l1)               \
  SP(TR, msg, l2)               \
  SP(IT, msg, l3)               \
  SP(ES, msg, l4)
#include "TranslationStrings.h"
#undef TR
#undef SP

// Translation table of pointers to translation strings in PROGMEM
const char *const translationTable[NUM_MESSAGES][LANG_COUNT] PROGMEM = {
#define TR(msg, ...) {tr_msg_##msg##_##EN, tr_msg_##msg##_##TR, tr_msg_##msg##_##IT, tr_msg_##msg##_##ES},
#include "TranslationStrings.h"
#undef TR
}; // translationTable

// Number of messages to translate
#define NUM_MESSAGES_TRANSLATE (sizeof(translationTable) / sizeof(translationTable[0]))
_Static_assert(NUM_MESSAGES_TRANSLATE == NUM_MESSAGES, "Translation table size does not match number of messages");

// language code comma separated string in PROGMEM
static const char languageCodeTable[] = {
#define X(lang, code, name) code ","
    LANGUAGELIST
#undef X
};

// language name comma separated string in PROGMEM
static const char languageNameTable[] PROGMEM = {
#define X(lang, code, name) name ","
    LANGUAGELIST
#undef X
};

// Temporary buffer for reading strings from PROGMEM
#define MAX_TRANSLATION_LEN 128
static char translationBuffer[MAX_TRANSLATION_LEN];

String findWordInCommaList(const String &list, int index, int maxwords)
{
  String retstr = "";
  int begin = 0, end = 0;
  // int orgindex = index;
  if ((index < maxwords) && (index >= 0))
  {
    // iterate through comma separated list
    while (index >= 0)
    {
      end = list.indexOf(',', begin);
      if (end == -1)
        end = list.length();
      if (index-- == 0)
        break;
      begin = end + 1;
    }
    retstr = list.substring(begin, end);
  }
  // Serial.printf_P(PSTR("findWordInCommaList list='%s' index=%d -> '%s'\n"), list.c_str(), orgindex, retstr.c_str());
  return retstr;
}

/**
 * @brief Get two-letter language code string from lang_t enum
 *
 * @param lang
 * @return const char*
 */
const char *getLanguageCode(lang_t lang)
{
  static char code[4];
  strncpy(code, findWordInCommaList(languageCodeTable, lang, NUM_LANGUAGES).c_str(), sizeof(code));
  code[sizeof(code) - 1] = 0; // ensure null termination
  Serial.printf_P(PSTR("getLanguageCode lang=%d: %s\n"), lang, code);
  return (code[0] != 0) ? code : "en"; // default to "en" if not found
}

/**
 * @brief Get lang_t enum from two-letter language code string
 *
 * @param code
 * @return lang_t
 */
lang_t getLanguageIdFromCode(const char *code)
{
  if (code != nullptr)
  {
    String codes = FPSTR(languageCodeTable);
    for (int i = 0; i < NUM_LANGUAGES; i++)
    {
      String langcode = codes.substring(i * 3, i * 3 + 2);
      if (langcode.equalsIgnoreCase(String(code)))
      {
        return static_cast<lang_t>(i);
      }
    }
  }
  return LANG_EN; // default to English if not found
}

/**
 * @brief Get language name string from lang_t enum
 *
 * @param lang
 * @return const char*
 */
const char *getLanguageName(lang_t lang)
{
  static char name[16];
  String namestr = FPSTR(languageNameTable);
  namestr = findWordInCommaList(namestr, lang, NUM_LANGUAGES);
  Serial.printf_P(PSTR("getLanguageName lang=%d: %s\n"), lang, namestr.c_str());
  strncpy(name, namestr.c_str(), sizeof(name));
  name[sizeof(name) - 1] = 0; // ensure null termination
  return name;
}

const char *getTranslation(int msg_id)
{
  return getTranslation(msg_id, currentLanguage);
}

const char *getTranslation(int msg_id, lang_t lang)
{
  translationBuffer[0] = 0; // clear buffer
  // Serial.printf_P(PSTR("getTranslation msg_id=%d lang=%d\n"), msg_id, lang);
  if ((msg_id >= 0) && (msg_id < NUM_MESSAGES) && (lang >= 0) && (lang < NUM_LANGUAGES))
  {
    memcpy_P(translationBuffer, translationTable[msg_id][lang], MAX_TRANSLATION_LEN);
    // Serial.println(translationBuffer);
    return translationBuffer;
  }
  return "unknown";
}

/**
 * @brief Retrieves the translated string for a given message ID in the current language.
 *
 * This function calls getTranslation with the specified message ID and the current language,
 * then converts the result to a String object.
 *
 * @param msg_id The identifier of the message to be translated.
 * @return String The translated message as a String object.
 */
String getTranslationStr(int msg_id)
{
  return String(getTranslation(msg_id, currentLanguage));
}

/**
 * @brief Retrieves the translated string for a given message ID and language.
 *
 * This function calls getTranslation with the specified message ID and language,
 * and returns the result as a String object.
 *
 * @param msg_id The identifier of the message to translate.
 * @param lang The language in which to retrieve the translation.
 * @return String containing the translated message.
 */
String getTranslationStr(int msg_id, lang_t lang)
{
  return String(getTranslation(msg_id, lang));
}

/**
 * @brief Sets the current language for the application.
 *
 * This function updates the global language setting if the provided language
 * identifier is within the valid range of supported languages.
 *
 * @param lang The language identifier to set (must be between 0 and NUM_LANGUAGES - 1).
 */
void setCurrentLanguageId(lang_t lang)
{
  if (lang >= 0 && lang < NUM_LANGUAGES)
  {
    currentLanguage = lang;
  }
}

/**
 * @brief Retrieves the currently selected language.
 *
 * @return lang_t The current language setting.
 */
lang_t getCurrentLanguageId()
{
  return currentLanguage;
}

// EOF
