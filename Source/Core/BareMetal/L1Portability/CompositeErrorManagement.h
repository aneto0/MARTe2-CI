	/**
 * @file CompoisteErrorManagement.h
 * @brief Header file for module ErrorManagement
 * @date 25/08/2018
 * @author Filippo Sartori
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the module CompositeErrorManagement
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef COMPOSITE_ERRORMANAGEMENT_H_
#define COMPOSITE_ERRORMANAGEMENT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ErrorManagement.h"
#include "DynamicCString.h"
#include "StaticCString.h"

/*---------------------------------------------------------------------------*/
/*                           Module declaration                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

namespace ErrorManagement {

template <class stringtype>
class ErrorReportBuilder {
public:
	template <class T1>
	static inline void CompositeReportError(const ErrorType &code,
                         	 	 	 	 	 T1 part1,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName);

	template <class T1,class T2>
	static inline void CompositeReportError(const ErrorType &code,
                         	 	 	 	 	 T1 part1,
                         	 	 	 	 	 T2 part2,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName);

	template <class T1,class T2,class T3>
	static inline void CompositeReportError(const ErrorType &code,
                         	 	 	 	 	 T1 part1,
                         	 	 	 	 	 T2 part2,
                         	 	 	 	 	 T3 part3,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName);

	template <class T1,class T2,class T3,class T4>
	static inline void CompositeReportError(const ErrorType &code,
	 	 	 	 	 	 	 	 	 	 	 T1 part1,
											 T2 part2,
											 T3 part3,
                         	 	 	 	 	 T4 part4,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName);

	template <class T1,class T2,class T3,class T4,class T5>
	static inline void CompositeReportError(const ErrorType &code,
	 	 	 	 	 	 	 	 	 	 	 T1 part1,
											 T2 part2,
											 T3 part3,
                         	 	 	 	 	 T4 part4,
                         	 	 	 	 	 T5 part5,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName);

	template <class T1,class T2,class T3,class T4,class T5,class T6>
	static inline void CompositeReportError(const ErrorType &code,
	 	 	 	 	 	 	 	 	 	 	 T1 part1,
											 T2 part2,
											 T3 part3,
                         	 	 	 	 	 T4 part4,
                         	 	 	 	 	 T5 part5,
                         	 	 	 	 	 T6 part6,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName);

	template <class T1,class T2,class T3,class T4,class T5,class T6,class T7>
	static inline void CompositeReportError(const ErrorType &code,
	 	 	 	 	 	 	 	 	 	 	 T1 part1,
											 T2 part2,
											 T3 part3,
                         	 	 	 	 	 T4 part4,
                         	 	 	 	 	 T5 part5,
                         	 	 	 	 	 T6 part6,
											 T7 part7,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName);

	template <class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8>
	static inline void CompositeReportError(const ErrorType &code,
	 	 	 	 	 	 	 	 	 	 	 T1 part1,
											 T2 part2,
											 T3 part3,
                         	 	 	 	 	 T4 part4,
                         	 	 	 	 	 T5 part5,
                         	 	 	 	 	 T6 part6,
											 T7 part7,
											 T8 part8,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName);

	template <class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9>
	static inline void CompositeReportError(const ErrorType &code,
	 	 	 	 	 	 	 	 	 	 	 T1 part1,
											 T2 part2,
											 T3 part3,
                         	 	 	 	 	 T4 part4,
                         	 	 	 	 	 T5 part5,
                         	 	 	 	 	 T6 part6,
											 T7 part7,
											 T8 part8,
											 T9 part9,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName);

	template <class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10>
	static inline void CompositeReportError(const ErrorType &code,
	 	 	 	 	 	 	 	 	 	 	 T1 part1,
											 T2 part2,
											 T3 part3,
                         	 	 	 	 	 T4 part4,
                         	 	 	 	 	 T5 part5,
                         	 	 	 	 	 T6 part6,
											 T7 part7,
											 T8 part8,
											 T9 part9,
											 T10 part10,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName);

	}; // ErrorReportBuilder<>

} // ErrorManagement

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace ErrorManagement {


template <class stringtype>
template <class T1>
void ErrorReportBuilder<stringtype>::CompositeReportError(const ErrorType &code,
	 	 	 	 	 	 	 	 	 	 	 T1 part1,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName){
	if (!code) {
		DynamicCString err;
		err().Append(part1);
		ConditionalReportError(code,err,fileName,lineNumber,functionName);
	}
}

template <class stringtype>
template <class T1,class T2>
void ErrorReportBuilder<stringtype>::CompositeReportError(const ErrorType &code,
	 	 	 	 	 	 	 	 	 	 	 T1 part1,
											 T2 part2,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName){
	if (!code) {
		stringtype err;
		err().Append(part1).Append(part2);
		ConditionalReportError(code,err,fileName,lineNumber,functionName);
	}
}

template <class stringtype>
template <class T1,class T2,class T3>
void ErrorReportBuilder<stringtype>::CompositeReportError(const ErrorType &code,
	 	 	 	 	 	 	 	 	 	 	 T1 part1,
											 T2 part2,
											 T3 part3,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName){
	if (!code) {
		stringtype err;
		err().Append(part1).Append(part2).Append(part3);
		ConditionalReportError(code,err,fileName,lineNumber,functionName);
	}
}

template <class stringtype>
template <class T1,class T2,class T3,class T4>
void ErrorReportBuilder<stringtype>::CompositeReportError(const ErrorType &code,
											T1 part1,
											T2 part2,
											T3 part3,
											T4 part4,
											CCString const fileName,
											const int16 lineNumber,
											CCString const functionName){
	if (!code) {
		stringtype err;
		err().Append(part1).Append(part2).Append(part3).Append(part4);
		ConditionalReportError(code,err,fileName,lineNumber,functionName);
	}
}

template <class stringtype>
template <class T1,class T2,class T3,class T4,class T5>
void ErrorReportBuilder<stringtype>::CompositeReportError(const ErrorType &code,
											T1 part1,
											T2 part2,
											T3 part3,
											T4 part4,
											T5 part5,
											CCString const fileName,
											const int16 lineNumber,
											CCString const functionName){
	if (!code) {
		stringtype err;
		err().Append(part1).Append(part2).Append(part3).Append(part4).Append(part5);
		ConditionalReportError(code,err,fileName,lineNumber,functionName);
	}
}

template <class stringtype>
template <class T1,class T2,class T3,class T4,class T5,class T6>
void ErrorReportBuilder<stringtype>::CompositeReportError(const ErrorType &code,
											T1 part1,
											T2 part2,
											T3 part3,
											T4 part4,
											T5 part5,
											T6 part6,
											CCString const fileName,
											const int16 lineNumber,
											CCString const functionName){
	if (!code) {
		stringtype err;
		err().Append(part1).Append(part2).Append(part3).Append(part4).Append(part5).Append(part6);
		ConditionalReportError(code,err,fileName,lineNumber,functionName);
	}
}

template <class stringtype>
template <class T1,class T2,class T3,class T4,class T5,class T6,class T7>
void ErrorReportBuilder<stringtype>::CompositeReportError(const ErrorType &code,
											T1 part1,
											T2 part2,
											T3 part3,
											T4 part4,
											T5 part5,
											T6 part6,
											T7 part7,
											CCString const fileName,
											const int16 lineNumber,
											CCString const functionName){
	if (!code) {
		stringtype err;
		err().Append(part1).Append(part2).Append(part3).Append(part4).Append(part5).Append(part6).Append(part7);
		ConditionalReportError(code,err,fileName,lineNumber,functionName);
	}
}

template <class stringtype>
template <class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8>
void ErrorReportBuilder<stringtype>::CompositeReportError(const ErrorType &code,
											T1 part1,
											T2 part2,
											T3 part3,
											T4 part4,
											T5 part5,
											T6 part6,
											T7 part7,
											T8 part8,
											CCString const fileName,
											const int16 lineNumber,
											CCString const functionName){
	if (!code) {
		stringtype err;
		err().Append(part1).Append(part2).Append(part3).Append(part4).Append(part5).Append(part6).Append(part7).Append(part8);
		ConditionalReportError(code,err,fileName,lineNumber,functionName);
	}
}

template <class stringtype>
template <class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9>
void ErrorReportBuilder<stringtype>::CompositeReportError(const ErrorType &code,
											T1 part1,
											T2 part2,
											T3 part3,
											T4 part4,
											T5 part5,
											T6 part6,
											T7 part7,
											T8 part8,
											T9 part9,
											CCString const fileName,
											const int16 lineNumber,
											CCString const functionName){
	if (!code) {
		stringtype err;
		err().Append(part1).Append(part2).Append(part3).Append(part4).Append(part5).Append(part6).Append(part7).Append(part8).Append(part9);
		ConditionalReportError(code,err,fileName,lineNumber,functionName);
	}
}

template <class stringtype>
template <class T1,class T2,class T3,class T4,class T5,class T6,class T7,class T8,class T9,class T10>
void ErrorReportBuilder<stringtype>::CompositeReportError(const ErrorType &code,
											T1 part1,
											T2 part2,
											T3 part3,
											T4 part4,
											T5 part5,
											T6 part6,
											T7 part7,
											T8 part8,
											T9 part9,
											T10 part10,
											CCString const fileName,
											const int16 lineNumber,
											CCString const functionName){
	if (!code) {
		stringtype err;
		err().Append(part1).Append(part2).Append(part3).Append(part4).Append(part5).Append(part6).Append(part7).Append(part8).Append(part9).Append(part10);
		ConditionalReportError(code,err,fileName,lineNumber,functionName);
	}
}

template<uint32 size>
class StaticCStringS: public MARTe::StaticCString<size>{
	char8 buffer[size];
public:
	StaticCStringS(): StaticCString(buffer){buffer[0] = '\0';}
};

} // namespace ErrorManagement


#define COMPOSITE_REPORT_ERROR_STATIC(code,size,...)															  \
MARTe::ErrorManagement::ErrorReportBuilder<MARTe::ErrorManagement::StaticCStringS<size>>::CompositeReportError(code,__VA_ARGS__,__FILE__,__LINE__,__ERROR_FUNCTION_NAME__);

#define COMPOSITE_REPORT_ERROR(code,...)															  \
MARTe::ErrorManagement::ErrorReportBuilder<MARTe::DynamicCString>::CompositeReportError(code,__VA_ARGS__,__FILE__,__LINE__,__ERROR_FUNCTION_NAME__);


} //MARTe
#endif /* ERRORMANAGEMENT_H_ */
