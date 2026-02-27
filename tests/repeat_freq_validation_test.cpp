#include "../examples/companion_radio/RepeatFreqValidation.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void teste_einzelfrequenz_preset_erlaubt_repeat() {
  FreqRange presets[] = {
    {433000, 433000},
    {869000, 869000},
  };

  RepeatFreqRegel regel = RepeatFreqRegel::KEIN_TREFFER;
  bool erlaubt = istRepeatFreqErlaubt(433000, presets, sizeof(presets) / sizeof(presets[0]), &regel);
  assert(erlaubt);
  assert(regel == RepeatFreqRegel::EINZELFREQUENZ);
}

static void teste_intervall_erlaubt_frequenzen_innerhalb() {
  FreqRange intervalle[] = {
    {915000, 928000},
  };

  RepeatFreqRegel regel = RepeatFreqRegel::KEIN_TREFFER;
  bool erlaubt = istRepeatFreqErlaubt(920000, intervalle, sizeof(intervalle) / sizeof(intervalle[0]), &regel);
  assert(erlaubt);
  assert(regel == RepeatFreqRegel::INTERVALL);
}

static void teste_ausserhalb_blockiert_repeat_mit_trefferregel() {
  FreqRange kombi[] = {
    {433000, 433000},
    {915000, 928000},
  };

  RepeatFreqRegel regel = RepeatFreqRegel::EINZELFREQUENZ;
  bool erlaubt = istRepeatFreqErlaubt(500000, kombi, sizeof(kombi) / sizeof(kombi[0]), &regel);
  assert(!erlaubt);
  assert(regel == RepeatFreqRegel::KEIN_TREFFER);
}

static void teste_ungueltiger_bereich_wird_nicht_als_intervall_genutzt() {
  FreqRange ungueltig[] = {
    {928000, 915000},
  };

  RepeatFreqRegel regel = RepeatFreqRegel::EINZELFREQUENZ;
  bool erlaubt = istRepeatFreqErlaubt(920000, ungueltig, sizeof(ungueltig) / sizeof(ungueltig[0]), &regel);
  assert(!erlaubt);
  assert(regel == RepeatFreqRegel::UNGUELTIGER_BEREICH);
}

static void teste_formatierung_von_freq_bereichen() {
  char puffer[32];

  FreqRange einzeln{433000, 433000};
  FreqRange intervall{915000, 928000};

  assert(strcmp(formatteFreqBereichMHz(einzeln, puffer, sizeof(puffer)), "433.0 MHz") == 0);
  assert(strcmp(formatteFreqBereichMHz(intervall, puffer, sizeof(puffer)), "915.0–928.0 MHz") == 0);
}

int main() {
  teste_einzelfrequenz_preset_erlaubt_repeat();
  teste_intervall_erlaubt_frequenzen_innerhalb();
  teste_ausserhalb_blockiert_repeat_mit_trefferregel();
  teste_ungueltiger_bereich_wird_nicht_als_intervall_genutzt();
  teste_formatierung_von_freq_bereichen();
  puts("Alle Repeat-Frequenz-Tests erfolgreich.");
  return 0;
}
