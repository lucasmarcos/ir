#include <IRLibDecodeBase.h>
#include <IRLib_HashRaw.h>
#include <IRLibCombo.h>

#include <IRLibRecvPCI.h>

#include <Keyboard.h>
#include <Mouse.h>

IRdecode decoder;

IRrecvPCI receiver = IRrecvPCI(2);

uint32_t last_value = 0;
uint32_t last_time = 0;

int8_t mouse_step;

void setup()
{
  Serial.begin(9600);
  Keyboard.begin();
  Mouse.begin();

  receiver.enableIRIn();

  randomSeed(analogRead(0));
}

void loop()
{
  mouse_step = random(10, 100);
  
  if(receiver.getResults()) {
    decoder.decode();
    Serial.println(decoder.value, HEX);

    if(decoder.value == last_value) {
      if((millis() - last_time) > 8) last_value = 0;
    } else {;
      last_value = decoder.value;
      last_time = millis();

      Serial.println(mouse_step);

      switch(decoder.value) {
        case 0x922F5983: // toshiba play
        case 0xD388DDD:  // az play
        case 0x75C0C3EC: // panasonic src
          Keyboard.write(' ');
          break;

        case 0x6F46633F: // toshiba info
        case 0x58C35B9:  // az info
        case 0x46C1822C: // panasonic sel
          Keyboard.write('o');
          break;

        case 0xC8F3BB43: // toshiba left
        case 0xDCAD008C: // panasonic 7
          Keyboard.write(0xD8);
          break;

        case 0xBDFF627F: // toshiba right
        case 0x887615CE: // panasonic 9
          Keyboard.write(0xD7);
          break;

        case 0x4A8B53A:  // toshiba down
        case 0x5C40BC74: // panasonic 0
          Keyboard.write(0xD9);
          break;

        case 0x178D4008: // toshiba up
        case 0x7ED3C066: // panasonic 8
          Keyboard.write(0xDA);
          break;

        case 0x461E8C3A: // toshiba enter
        case 0x60F69F9:  // az enter
        case 0xE8E6F012: // panasonic band/set
          Keyboard.write(0xB0);
          break;

        case 0x4417AD7A: // toshiba back
          Keyboard.write(0xB1);
          break;

        case 0x3DDDE4EC: // panasonic mute
          Keyboard.write('n');
          break;

        case 0x13B0782A: // panasonic #
          Keyboard.write(',');
          break;

        case 0xE13A97EE: // panasonic disp
          Keyboard.write('.');
          break;

        case 0x7D6D1C10: // panasonic volup
          Keyboard.write('x');
          break;

        case 0xC406418A: // panasonic voldown
          Keyboard.write('z');
          break;

        case 0xB6804B0: // panasonic 1
          Mouse.click(MOUSE_LEFT);
          break;

        case 0x425DAA14: // panasonic 2
          Mouse.move(0, -mouse_step, 0);
          break;

        case 0x884FD9B0: // panasonic 3
          Mouse.click(MOUSE_RIGHT);
          break;

        case 0xCD5B41EA: // panasonic 4
          Mouse.move(-mouse_step, 0, 0);
          break;

        case 0x39EE944A: // panasonic 5
          Mouse.move(0, mouse_step, 0);
          break;

        case 0xD84F9AAE: // panasonic 6
          Mouse.move(mouse_step, 0, 0);
          break;
      }
    }

    receiver.enableIRIn();
  }
}
