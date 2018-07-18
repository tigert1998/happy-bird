#pragma once

class Attenuation{
private:
	float range_;
	float constant_;
	float linear_;
	float quadratic_;
	
public:
	Attenuation(float range);
	float range() const;
	float constant() const;
	float linear() const;
	float quadratic() const;
};