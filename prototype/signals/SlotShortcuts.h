/*
 * SlotShortcuts.h
 *
 *  Created on: 19.01.2013
 *      Author: night
 */

#ifndef SLOTSHORTCUTS_H_
#define SLOTSHORTCUTS_H_

#include "ObjectTypes.h"

#define QU_THIS_SLOT(varname, method)       \
        prototype::Slot<                    \
            prototype::MemberSignatureExtractor< decltype(& QU_THIS_TYPE::method) >::type        \
        > varname {this, & QU_THIS_TYPE::method}

#define QU_THIS_OVERLOADSLOT(varname, method, signature, ...)    \
        prototype::Slot< signature > varname {this,         \
                   prototype::MemberFunctionCast<QU_THIS_TYPE, signature, ##__VA_ARGS__>::cast(& QU_THIS_TYPE::method)       \
        }

#define QU_THIS_OTHERSLOT(varname, slotSignature, method)   \
        prototype::Slot< slotSignature > varname {this, & QU_THIS_TYPE::method}

#define QU_THIS_OTHER_OVERLOADSLOT(varname, slotSignature, method, signature, ...)    \
        prototype::Slot< slotSignature > varname {this,         \
                   prototype::MemberFunctionCast<QU_THIS_TYPE, signature, ##__VA_ARGS__>::cast(& QU_THIS_TYPE::method)       \
        }

#define QU_SLOT(obj, varname, method)       \
        prototype::Slot<                    \
            prototype::MemberSignatureExtractor< decltype(& QU_OBJ_TYPE(obj)::method) >::type        \
        > varname {obj, & QU_OBJ_TYPE(obj)::method};

#define QU_OVERLOADSLOT(obj, varname, method, signature, ...)    \
        prototype::Slot< signature > varname {obj,         \
                   prototype::MemberFunctionCast<QU_OBJ_TYPE(obj), signature, ##__VA_ARGS__>::cast(& QU_OBJ_TYPE(obj)::method)       \
        }

#define QU_OTHERSLOT(obj, varname, slotSignature, method)   \
        prototype::Slot< slotSignature > varname {obj, & QU_OBJ_TYPE(obj)::method};

#define QU_OTHER_OVERLOADSLOT(obj,varname, slotSignature, method, signature, ...)    \
        prototype::Slot< slotSignature > varname {obj,         \
                   prototype::MemberFunctionCast<QU_OBJ_TYPE(obj), signature, ##__VA_ARGS__>::cast(& QU_OBJ_TYPE(obj)::method)       \
        }

#define QU_STATIC_SLOT(varname, function)   \
        prototype::Slot<                    \
            prototype::FunctionSignatureExtractor<decltype(function)>::type     \
        > varname {function}

#define QU_STATIC_OTHERSLOT(varname, slotSignature, function)       \
        prototype::Slot< slotSignature > varname {function}

#define QU_STATIC_OVERLOADSLOT(varname, function, signature)        \
        prototype::Slot< signature > varname {                      \
            prototype::SignaturePointerCast<signature>::cast(function)      \
        }

#define QU_STATIC_OTHER_OVERLOADSLOT(varname, slotSignature, function, signature)   \
        prototype::Slot< slotSignature > varname {                                 \
            prototype::SignaturePointerCast<signature>::cast(function)                         \
        }


#endif /* SLOTSHORTCUTS_H_ */
