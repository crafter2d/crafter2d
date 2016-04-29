
#ifndef SCRIPT_MACROS_H
#define SCRIPT_MACROS_H

#define GET_THIS(type, variable)                   type& variable = accessor.getObject(0)->get<type>()
#define DESTRUCT_THIS(type)                        delete accessor.getObject(0)->use<type>();

#define RETURN_CLASS(class, pointer)               accessor.setResult(class, pointer, false)
#define RETURN_CLASS_OWNED(class, pointer)         accessor.setResult(class, pointer, true)

#define RETURN_CLASS_I(scriptable)                 accessor.setResult(scriptable, false);

#endif // SCRIPT_MACROS_H
