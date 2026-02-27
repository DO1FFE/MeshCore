#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

enum class RepeatFreqRegel : uint8_t {
  EINZELFREQUENZ,
  INTERVALL,
  UNGUELTIGER_BEREICH,
  KEIN_TREFFER
};

struct FreqRange {
  uint32_t lower_freq;
  uint32_t upper_freq;
};

inline bool istGueltigerFreqBereich(const FreqRange &bereich) {
  return bereich.lower_freq <= bereich.upper_freq;
}

inline bool istRepeatFreqErlaubt(uint32_t ausgewaehlte_freq,
                                 const FreqRange *erlaubte_bereiche,
                                 size_t anzahl_bereiche,
                                 RepeatFreqRegel *angewendete_regel = nullptr) {
  bool hat_ungueltigen_bereich = false;

  for (size_t i = 0; i < anzahl_bereiche; i++) {
    const auto &bereich = erlaubte_bereiche[i];

    if (!istGueltigerFreqBereich(bereich)) {
      hat_ungueltigen_bereich = true;
      continue;
    }

    if (bereich.lower_freq == bereich.upper_freq) {
      if (ausgewaehlte_freq == bereich.lower_freq) {
        if (angewendete_regel) *angewendete_regel = RepeatFreqRegel::EINZELFREQUENZ;
        return true;
      }
      continue;
    }

    if (ausgewaehlte_freq >= bereich.lower_freq && ausgewaehlte_freq <= bereich.upper_freq) {
      if (angewendete_regel) *angewendete_regel = RepeatFreqRegel::INTERVALL;
      return true;
    }
  }

  if (angewendete_regel) {
    *angewendete_regel = hat_ungueltigen_bereich ? RepeatFreqRegel::UNGUELTIGER_BEREICH
                                                 : RepeatFreqRegel::KEIN_TREFFER;
  }
  return false;
}

inline const char *formatteFreqBereichMHz(const FreqRange &bereich,
                                          char *ausgabe,
                                          size_t ausgabe_laenge) {
  if (ausgabe_laenge == 0) return ausgabe;

  if (bereich.lower_freq == bereich.upper_freq) {
    snprintf(ausgabe, ausgabe_laenge, "%.1f MHz", bereich.lower_freq / 1000.0f);
  } else {
    snprintf(ausgabe, ausgabe_laenge, "%.1f–%.1f MHz", bereich.lower_freq / 1000.0f,
             bereich.upper_freq / 1000.0f);
  }

  return ausgabe;
}
