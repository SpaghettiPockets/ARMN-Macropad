# ARMN-Macropad

Fire direktekoblede knapper for **Raspberry Pi Pico 2 W (RP2350)**.
Vanlig USB HID-tastatur, i rekkefølgen **A, R, M, N**. USB gir både strøm og data.
Ingen matrise eller dioder. Alle fire knapper kan holdes samtidig.
5 ms debounce på både trykk og slipp. Vanlig repetisjon styres av datamaskinen.
Dette er bokstavtaster: små bokstaver normalt, store med Shift/Caps Lock.
Bluetooth/Wi-Fi er ikke aktivert. Ingen automatisk firmware-deep-sleep.

## Lodding

Se kortet fra **komponentsiden med USB-kontakten øverst**.
Alle tilkoblingene nedenfor er på **høyre langside**, samme side som VBUS og VSYS.
Fysiske pinnumre er ikke det samme som GPIO-numre.

| Knapp i ønsket rekkefølge | GPIO | Fysisk pinne | Andre switchbein |
|---|---|---|---|
| 1 — A | GP16 | 21 | GND |
| 2 — R | GP18 | 24 | GND |
| 3 — M | GP20 | 26 | GND |
| 4 — N | GP22 | 29 | GND |

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
GP22 (pinne 29) ---- [ N ] ----+
```

Pinnevalget er kontrollert mot [Raspberry Pis offisielle Pico 2 W-pinout](https://pip-assets.raspberrypi.com/categories/1088-raspberry-pi-pico-2-w/documents/RP-008305-DS-1-pico-2-w-pinout.pdf).

## Flashing

1. Last ned **ARMN-Macropad-pico2w.uf2** fra repoets Releases.
   Alternativt: Actions → siste vellykkede bygg → artifact `ARMN-Macropad-pico2w`, og pakk ut ZIP-filen.
2. Koble Pico fra USB. Hold inne **BOOTSEL** mens du kobler til en USB-datakabel.
3. Slipp BOOTSEL når disken **RP2350** vises.
4. Kopier UF2-filen til denne disken. Kortet starter automatisk på nytt som `ARMN-Macropad`.
5. Åpne en teksteditor og prøv knappene: `a`, `r`, `m`, `n`.

BOOTSEL kan brukes igjen ved senere oppdateringer. Ingen programmerer eller Python-installasjon på kortet trengs.
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
