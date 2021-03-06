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


#ifndef CONNECTIVITY_ADO_WRAP_COLUMN_HXX
#define CONNECTIVITY_ADO_WRAP_COLUMN_HXX

#include "ado/Aolewrap.hxx"

#include "ado_pre_sys_include.h"
#include <adoint.h>
#include "ado_post_sys_include.h"

namespace connectivity
{
	namespace ado
	{
		class WpADOColumn : public WpOLEBase<_ADOColumn>
		{
		public:
			WpADOColumn(_ADOColumn* pInt=NULL)	:	WpOLEBase<_ADOColumn>(pInt){}
			WpADOColumn(const WpADOColumn& rhs){operator=(rhs);}

			void Create();

			inline WpADOColumn& operator=(const WpADOColumn& rhs)
				{WpOLEBase<_ADOColumn>::operator=(rhs); return *this;}

			::rtl::OUString get_Name() const;
			::rtl::OUString get_RelatedColumn() const;
			void put_Name(const ::rtl::OUString& _rName);
			void put_RelatedColumn(const ::rtl::OUString& _rName);
			DataTypeEnum get_Type() const;
			void put_Type(const DataTypeEnum& _eNum) ;
			sal_Int32 get_Precision() const;
			void put_Precision(sal_Int32 _nPre) ;
			sal_uInt8 get_NumericScale() const;
			void put_NumericScale(sal_Int8 _nScale);
			SortOrderEnum get_SortOrder() const;
			void put_SortOrder(SortOrderEnum _nScale);
			sal_Int32 get_DefinedSize() const;
			ColumnAttributesEnum get_Attributes() const;
			sal_Bool put_Attributes(const ColumnAttributesEnum& _eNum);
			WpADOProperties get_Properties() const;
			void put_ParentCatalog(/* [in] */ _ADOCatalog __RPC_FAR *ppvObject);
		};
	}
}

#endif //CONNECTIVITY_ADO_WRAP_COLUMN_HXX
