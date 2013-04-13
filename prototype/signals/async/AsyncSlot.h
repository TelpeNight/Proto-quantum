/*
 * AsyncSlot.h
 *
 *  Created on: 13.04.2013
 *      Author: night
 */

#ifndef ASYNCSLOT_H_
#define ASYNCSLOT_H_

#include "ContextId.h"

#include <signals/Slot.h>
#include <type_traits>

namespace prototype {
    namespace async {


        template<typename FunctionSignature>
        class AsyncSlot;

        template<typename RType, typename... ArgTypes>
        class AsyncSlot<RType (ArgTypes...)> {
            static_assert(!std::is_same<RType, void>::value,
                          "Async slot should return void");
        public:
            typedef RType ReturnType;
            typedef ReturnType (FunctionSignature)(ArgTypes...);
            typedef Slot<FunctionSignature> SlotType;

            AsyncSlot(const ContextId& id);
            AsyncSlot(const ContextId& id, const SlotType& slot);

            template<class OtherAsyncSlot>
            AsyncSlot(const OtherAsyncSlot& other);
            AsyncSlot(const AsyncSlot& other);
            AsyncSlot(AsyncSlot&& other);

            template<class OtherAsyncSlot>
            AsyncSlot& operator=(const OtherAsyncSlot& other);
            AsyncSlot& operator=(const AsyncSlot& other);
            AsyncSlot& operator=(AsyncSlot&& other);

            explicit operator bool() const;
            bool isEmpty();

            SlotType getOrigSlot() const;
            SlotType getAsyncSlot() const;
            operator SlotType() const {return getAsyncSlot();}

            void invoke(ArgTypes&&...);
            void operator()(ArgTypes&&...);

        private:
            ContextId _id;
            SlotType _slot;
        };

    } /* namespace async */
} /* namespace prototype */
#endif /* ASYNCSLOT_H_ */
