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
        ContextId() : _id(0){}
        int _id;
    };

}}


#endif /* CONTEXTID_H_ */
