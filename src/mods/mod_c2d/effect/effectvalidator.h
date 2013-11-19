
#ifndef EFFECT_VALIDATOR_H
#define EFFECT_VALIDATOR_H

class ASTEffect;

class EffectValidator
{
public:

 // validation
   void validate(const ASTEffect& effect);

private:

 // validation
   void validateFunctions(const ASTEffect& effect);
   void validateTechniques(const ASTEffect& effect);

};

#endif // EFFECT_VALIDATOR_H
