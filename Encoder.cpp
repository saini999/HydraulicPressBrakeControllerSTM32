#include <Encoder.h>

Encoder::Encoder(uint8_t pinA, uint8_t pinB, callback_function_t callback){
  _pinA = pinA;
  _pinB = pinB;
  _callback = callback;
  pinMode(_pinA, INPUT);
  pinMode(_pinB, INPUT);
  attachInterrupt(_pinA, _callback, CHANGE);
  attachInterrupt(_pinB, _callback, CHANGE);
}

int32_t Encoder::read(){
    return _position;
}

void Encoder::write(int32_t pos){
    _position = pos;
}

void Encoder::setResolution(mm res){
	_res = res;
}

mm Encoder::getResolution(){
	return _res;
}

void Encoder::setCorrection(mm corr){
	_corr = corr;
}

mm Encoder::getCorrection(){
	return _corr;
}

mm Encoder::getPos(){
	return (_position * _res) + _corr;
}

void Encoder::update(void) {
		uint8_t s = _state & 3;
		if (digitalRead(_pinA)) s |= 4;
		if (digitalRead(_pinB)) s |= 8;
		switch (s) {
			case 0: case 5: case 10: case 15:
				break;
			case 1: case 7: case 8: case 14:
				_position++; break;
			case 2: case 4: case 11: case 13:
				_position--; break;
			case 3: case 12:
				_position += 2; break;
			default:
				_position -= 2; break;
		}
		_state = (s >> 2);
	}