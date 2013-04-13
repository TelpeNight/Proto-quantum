/*
 * FunctorHandler.h
 *
 *  Created on: 13.04.2013
 *      Author: night
 */

#ifndef FUNCTORHANDLER_H_
#define FUNCTORHANDLER_H_

namespace prototype {

    template<class T>
    bool compareFuctors(const T& t1, const T& t2) {
        return false;
    }

    template<class T>
    bool hasComarison() {
        return false;
    }

    template<class T>
    bool validateFuctors(const T& t) {
        return true;
    }

    class FunctionObjectHandler {
    public:
        virtual ~FunctionObjectHandler() {}
        virtual bool compare(const FunctionObjectHandler* pOther) const = 0;
        virtual bool validate() const = 0;
    };

    template<class T>
    struct FunctorHandler : FunctionObjectHandler {
        T functor;
        FunctorHandler(const T& t) : functor(t) {}

        virtual bool compare(const FunctionObjectHandler* pOther) const override {
            if (pOther == nullptr) {
                return false;
            }
            auto pCasted = dynamic_cast<const FunctorHandler*>(pOther);            //TODO change to as
            if (pCasted == nullptr) {
                return false;
            }
            return compareFuctors(functor, pCasted->functor);
        }

        virtual bool validate() const override {
            return validateFuctors(functor);
        }
    };
}


#endif /* FUNCTORHANDLER_H_ */
