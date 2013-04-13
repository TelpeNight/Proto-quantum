/*
 * ContextId.h
 *
 *  Created on: 13.04.2013
 *      Author: night
 */

#ifndef CONTEXTID_H_
#define CONTEXTID_H_

namespace prototype {
namespace async {

    class ContextManager;
    class ContextId {
        friend class ContextManager;
    public:
        ContextId(const ContextId& other): _id(other._id){}

        inline
        bool operator==(const ContextId& other) {return _id == other._id;}
    private:
        ContextId();
        int _id;
    };

}}


#endif /* CONTEXTID_H_ */
