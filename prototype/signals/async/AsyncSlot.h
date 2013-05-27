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
#include <utility>

namespace prototype {
    namespace async {

        namespace internal {
            struct AsyncSlotTag{};
        }

        template<typename FunctionSignature>
        class AsyncSlot;

        template<typename RType, typename... ArgTypes>
        class AsyncSlot<RType (ArgTypes...)> {
            static_assert(std::is_same<RType, void>::value,
                          "Async slot should return void");
        public:
            typedef RType ReturnType;
            typedef ReturnType (FunctionSignature)(ArgTypes...);
            typedef Slot<FunctionSignature> SlotType;
            typedef internal::AsyncSlotTag Tag;

            AsyncSlot(const ContextId& id) : _contextId(id){}
            AsyncSlot(const ContextId& id, const SlotType& slot) : _contextId(id), _slot(slot){}
            AsyncSlot(const ContextId& id, SlotType&& slot) : _contextId(id), _slot(slot){}

            template<class OtherAsyncSlot,
                EnableIf<
                    std::is_same<
                        typename RemoveRef<OtherAsyncSlot>::Tag,
                        internal::AsyncSlotTag
                    >
                > = 0,
                DisableIf<
                    std::is_same<
                        RemoveRef<OtherAsyncSlot>,
                        AsyncSlot<RType (ArgTypes...)>
                    >
                > = 0
            >
            AsyncSlot(OtherAsyncSlot&& other);
            AsyncSlot(const AsyncSlot& other);
            AsyncSlot(AsyncSlot&& other);

            template<class OtherAsyncSlot>
            AsyncSlot& operator=(OtherAsyncSlot&& other);
            AsyncSlot& operator=(const AsyncSlot& other);
            AsyncSlot& operator=(AsyncSlot&& other);

            bool isEmpty();

            SlotType getOrigSlot() const;
            SlotType getAsyncSlot() const;
            operator SlotType() const {return getAsyncSlot();}

            void invoke(ArgTypes&&...);
            void operator()(ArgTypes&&...);

        //private:
            ContextId _contextId;
            SlotType _slot;
        };

        template<typename RType, typename... ArgTypes>
        AsyncSlot<RType (ArgTypes...)>::AsyncSlot(const AsyncSlot& other) :
            AsyncSlot(other._contextId, other._slot) {}

        template<typename RType, typename... ArgTypes>
        AsyncSlot<RType (ArgTypes...)>::AsyncSlot(AsyncSlot&& other) :
            AsyncSlot(other._contextId, std::move(other._slot)) {}

        template<typename RType, typename... ArgTypes>
        template<class OtherAsyncSlot,
            EnableIf<
                std::is_same<
                    typename RemoveRef<OtherAsyncSlot>::Tag,
                    internal::AsyncSlotTag
                >
            >,
            DisableIf<
                std::is_same<
                    RemoveRef<OtherAsyncSlot>,
                    AsyncSlot<RType (ArgTypes...)>
                >
            >
        >
        AsyncSlot<RType (ArgTypes...)>::AsyncSlot(OtherAsyncSlot&& other) :
            AsyncSlot(other._contextId, other._slot) {}

    } /* namespace async */
} /* namespace prototype */
#endif /* ASYNCSLOT_H_ */
