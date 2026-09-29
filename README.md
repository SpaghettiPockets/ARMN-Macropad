# ARMN-Macropad

Fire direktekoblede knapper for **Raspberry Pi Pico 2 W (RP2350)**.
Vanlig USB HID-tastatur, i rekkefølgen **A, R, M, N**. USB gir både strøm og data.
Ingen matrise eller dioder. Alle fire knapper kan holdes samtidig.
5 ms debounce på både trykk og slipp. Vanlig repetisjon styres av datamaskinen.
Dette er bokstavtaster: små bokstaver normalt, store med Shift/Caps Lock.
Bluetooth/Wi-Fi er ikke aktivert. Ingen automatisk firmware-deep-sleep.

## Lodding

Se kortet fra **komponentsiden med USB-kontakten øverst**.
Alle switchtilkoblingene nedenfor er på **høyre langside**, samme side som VBUS og VSYS.
Fysiske pinnumre er ikke det samme som GPIO-numre.

| Knapp i ønsket rekkefølge | GPIO | Fysisk pinne | Andre switchbein |
|---|---|---|---|
| 1 — A | GP16 | 21 | GND |
| 2 — R | GP18 | 24 | GND |
| 3 — M | GP20 | 26 | GND |
| 4 — N | GP28 / A2 | 34 | GND |

Fra **v1.0.1** bruker N-knappen GP28 (fysisk pinne 34). GP28 fungerer her som vanlig
digital inngang; A2/ADC2 er en alternativ analogfunksjon som ikke brukes.
Den eldre **v1.0.0 bruker GP22 (pinne 29)**, så flash v1.0.1 ved denne ledningsføringen.
Ikke forveksle **GP28 (pinne 34)** med **fysisk pinne 28 (GND)**.

1. Koble fra USB før lodding.
2. Lodd ett elektrisk kontaktbein på hver switch til GPIO-en i tabellen.
3. Koble det andre kontaktbeinet på alle fire switchene sammen til en felles jordledning.
4. Lodd jordledningen til **GND, fysisk pinne 28**, mellom GP21 og GP22.
   GND på pinne 23 er også et alternativ på samme side.
5. Sjekk at det ikke er loddebroer mellom nabopinner.

Switchkontaktene har ingen polaritet. Interne pull-up-motstander er aktivert;
ingen eksterne motstander trengs for denne direktekoblingen.
**Ikke koble switchene til VBUS, VSYS eller 3V3.** VBUS og VSYS er strømpinner.

```text
GP16 (pinne 21) ---- [ A ] ----+
GP18 (pinne 24) ---- [ R ] ----+
GP20 (pinne 26) ---- [ M ] ----+---- GND (pinne 28)
GP28 (pinne 34) ---- [ N ] ----+
```

Pinnevalget er kontrollert mot [Raspberry Pis offisielle Pico 2 W-pinout](https://pip-assets.raspberrypi.com/categories/1088-raspberry-pi-pico-2-w/documents/RP-008305-DS-1-pico-2-w-pinout.pdf).

## USB-C-kontakt via testpads

Dette gjelder et **USB-C-hunnkontakt/breakout-kort for USB 2.0-data og 5 V**.
Den konkrete modellen er ikke identifisert: følg signalnavnene på kortet og produsentens
skjema, ikke en antatt venstre/høyre-rekkefølge på headeren. Et kort med bare strømuttak
uten D+ og D− kan ikke brukes som USB-tastaturtilkobling. Ikke bruk en PD-trigger som
forhandler frem høyere spenning enn 5 V.

### De fire ledningene

| USB-C-kortets signal | Lodd til på Pico 2 W | Funksjon |
|---|---|---|
| VBUS / 5V / V | **VBUS, fysisk pinne 40** | USB-strøm inn |
| GND / G | **TP1** | Jord ved USB-datalinjene |
| D− / D- / DM | **TP2** | USB data minus |
| D+ / DP | **TP3** | USB data pluss |

TP1–TP3 er testpads på **undersiden**. Finn dem etter TP-navnene i
[Pico 2 W-databladet, avsnitt 2.1 og komponenttegningen i vedlegg B](https://datasheets.raspberrypi.com/picow/pico-2-w-datasheet.pdf).
Når du snur kortet, speilvendes høyre/venstre sammenlignet med switchoppskriften over.
Pinne 40 er VBUS ved enden med den eksisterende USB-kontakten.
**TP5 er ikke VBUS på Pico 2 W.** La TP4, TP5 og TP6 være urørt i denne oppkoblingen.
Koble USB-C-kortets 5 V til VBUS som angitt, ikke til 3V3 eller en GPIO.

```text
USB-C-breakout                    Pico 2 W
VBUS / 5V ---------------------- VBUS (fysisk pinne 40)
GND ---------------------------- TP1
D-  ---------------------------- TP2
D+  ---------------------------- TP3
```

### CC1 og CC2 på USB-C-kortet

For en vanlig USB-C-hunnkontakt som skal fungere med en USB-C-til-USB-C-kabel,
må **CC1 og CC2 ha hver sin 5,1 kΩ-motstand til GND**. Mange breakout-kort har dem
allerede montert; kontroller skjemaet før du legger til flere.

```text
CC1 ---- [5,1 kΩ] ---- GND
CC2 ---- [5,1 kΩ] ---- GND
```

Hvis de mangler og CC-pinnene er tilgjengelige, monter én motstand fra hver CC-pin
til jord på breakout-kortet. **Ikke kortslutt CC1 og CC2 sammen**, og ikke koble dem
til Picoens GPIO-er. Har kortet bare fire loddepunkter, må du fortsatt bekrefte at
CC-motstandene finnes på kortet. Se et dokumentert eksempel hos
[Adafruit](https://www.adafruit.com/product/4090).
Hvis breakout-kortet eksponerer begge USB 2.0-datapar separat, skal A6/B6 (D+) kobles
sammen ved kontakten, og A7/B7 (D−) kobles sammen ved kontakten. På kort med ett D+/D−-par
er dette normalt allerede gjort; kontroller kortets skjema.

### Lodding og kontroll

1. Koble fra all strøm. Fortinn ledningsendene og testpadsene med litt tinn og flussmiddel.
2. Bruk korte, tynne, isolerte ledninger. Før D+ og D− sammen, gjerne som et lett tvunnet
   par, og hold lengden så kort som praktisk mulig inne i kabinettet.
3. Lodd etter tabellen. Bruk korte varmeøkter og unngå å dra i ledningen mens tinnet
   størkner; testpads tåler lite mekanisk belastning.
4. Fest USB-C-kortet mekanisk til kabinettet, og gi ledningene strekkavlastning slik
   at innsetting av kabelen ikke belaster Picoens testpads.
5. Med strømmen frakoblet: kontroller forbindelsene med multimeter og se etter
   loddebroer, særlig mellom D+/D− og mellom VBUS/GND. Ikke sett på strøm ved en
   vedvarende kortslutning mellom VBUS og GND.
6. Koble til datamaskinen gjennom **kun USB-C-kontakten** med en datakabel.
   Test både normal tastaturdrift og BOOTSEL-flashing. Test også begge orienteringer
   av USB-C-pluggen; feil i CC-/datakoblingen kan gi forskjellig resultat når pluggen snus.

**Bruk bare én USB-port om gangen.** Den nye kontakten deler VBUS og datalinjer med
Picoens eksisterende micro-USB-port. Ikke koble begge til datamaskin/strøm samtidig;
det kan forbinde to USB-strømkilder og to verter direkte.
Oppkoblingen krever ingen firmwareendring. Fysisk funksjon og ditt konkrete
USB-C-breakout-kort er ennå ikke verifisert.

## Flashing

1. Last ned **ARMN-Macropad-pico2w.uf2** fra repoets Releases.
   Alternativt: Actions → siste vellykkede bygg → artifact `ARMN-Macropad-pico2w`, og pakk ut ZIP-filen.
2. Koble Pico fra USB. Hold inne **BOOTSEL** mens du kobler til en USB-datakabel.
3. Slipp BOOTSEL når disken **RP2350** vises.
4. Kopier UF2-filen til denne disken. Kortet starter automatisk på nytt som `ARMN-Macropad`.
5. Åpne en teksteditor og prøv knappene: `a`, `r`, `m`, `n`.

BOOTSEL kan brukes igjen ved senere oppdateringer. Ingen programmerer eller Python-installasjon på kortet trengs.
Den nye USB-C-kontakten kan også brukes til flashing når den er koblet som beskrevet over.
Ikke bruk en UF2 for den eldre RP2040/Pico W.

## Verifisering på maskinvaren

Bygget og de automatiske testene bekrefter kompilering, tastetilordning,
debounce, samtidige trykk, slipperapport og tidtelleroverløp. Kortet er ikke fysisk testet av forfatteren.
Test korte trykk, hold/repetisjon, slipp, alle fire samtidig, og frakobling/ny tilkobling.
USB-vekking av en sovende datamaskin er ikke implementert.

## Bygg fra kildekoden

GitHub Actions bygger automatisk ved push. SDK er låst til Pico SDK 2.2.0,
commit `a1438dff1d38bd9c65dbd693f0e5db4b9ae91779`, med SDK-ens TinyUSB-versjon.
Linux med CMake, Ninja og ARM GCC:

```sh
git clone https://github.com/raspberrypi/pico-sdk.git pico-sdk
git -C pico-sdk checkout a1438dff1d38bd9c65dbd693f0e5db4b9ae91779
git -C pico-sdk submodule update --init lib/tinyusb
export PICO_SDK_PATH="$PWD/pico-sdk"
cmake -S . -B build -G Ninja -DPICO_BOARD=pico2_w -DPICO_PLATFORM=rp2350-arm-s -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

UF2 ligger i `build/armn_macropad.uf2`. SDK henter/verktøybygger picotool ved behov.
`src/keys.h` inneholder GPIO-er og HID-koder. Ikke endre disse uten også å oppdatere loddeoppskriften.
USB VID/PID kommer fra TinyUSBs eksempelområde og er ment for denne personlige prototypen.
