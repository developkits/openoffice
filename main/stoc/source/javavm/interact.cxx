/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_stoc.hxx"

#include "interact.hxx"

#include "com/sun/star/java/JavaDisabledException.hpp"
#include "com/sun/star/java/JavaVMCreationFailureException.hpp"
#include "com/sun/star/task/XInteractionAbort.hpp"
#include "com/sun/star/task/XInteractionRetry.hpp"
#include "com/sun/star/task/XInteractionContinuation.hpp"
#include "cppuhelper/implbase1.hxx"
#include "osl/mutex.hxx"

namespace css = com::sun::star;

using stoc_javavm::InteractionRequest;

namespace {

class AbortContinuation:
    public cppu::WeakImplHelper1< css::task::XInteractionAbort >
{
public:
    inline AbortContinuation() {}

    virtual inline void SAL_CALL select() throw (css::uno::RuntimeException) {}

private:
    AbortContinuation(AbortContinuation &); // not implemented
    void operator =(AbortContinuation); // not implemented

    virtual inline ~AbortContinuation() {}
};

}

class InteractionRequest::RetryContinuation:
    public cppu::WeakImplHelper1< css::task::XInteractionRetry >
{
public:
    inline RetryContinuation(): m_bSelected(false) {}

    virtual void SAL_CALL select() throw (css::uno::RuntimeException);

    bool isSelected() const;

private:
    RetryContinuation(RetryContinuation &); // not implemented
    void operator =(RetryContinuation); // not implemented

    virtual inline ~RetryContinuation() {}

    mutable osl::Mutex m_aMutex;
    bool m_bSelected;
};

void SAL_CALL InteractionRequest::RetryContinuation::select()
    throw (css::uno::RuntimeException)
{
    osl::MutexGuard aGuard(m_aMutex);
    m_bSelected = true;
}

bool InteractionRequest::RetryContinuation::isSelected() const
{
    osl::MutexGuard aGuard(m_aMutex);
    return m_bSelected;
}

InteractionRequest::InteractionRequest(css::uno::Any const & rRequest):
    m_aRequest(rRequest)
{
    m_aContinuations.realloc(2);
    m_xRetryContinuation = new RetryContinuation;
    m_aContinuations[0] = new AbortContinuation;
    m_aContinuations[1] = m_xRetryContinuation.get();
}

css::uno::Any SAL_CALL InteractionRequest::getRequest()
    throw (css::uno::RuntimeException)
{
    return m_aRequest;
}

css::uno::Sequence< css::uno::Reference< css::task::XInteractionContinuation > >
SAL_CALL InteractionRequest::getContinuations()
    throw (css::uno::RuntimeException)
{
    return m_aContinuations;
}

bool InteractionRequest::retry() const
{
    return m_xRetryContinuation.is() && m_xRetryContinuation->isSelected();
}

InteractionRequest::~InteractionRequest()
{}
