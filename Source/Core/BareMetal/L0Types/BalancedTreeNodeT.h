/*
 * BalancedTreeNodeT.h
 *
 *  Created on: 15 Jun 2017
 *      Author: sartofi
 */

#ifndef BALANCEDTREENODET_H_
#define BALANCEDTREENODET_H_


#include "BalancedTreeNode.h"
#include "GenericIterator.h"

namespace MARTe{

/**
 *
 */
template <class loadClass,class keyClass, typename loadKey>
class  BalancedTreeNodeT : public BalancedTreeNode{
public:
	/**
	 *
	 */
	BalancedTreeNodeT(const loadClass &payLoadIn){
		payLoad = payLoadIn;
	}

	/**
	 *
	 */
	virtual ~BalancedTreeNodeT(){
	}

	/**
	 * navigates tree smaller to greater
	 * returns ptr to node
	 */
	inline BalancedTreeNodeT<loadClass,keyClass,loadKey> *Seek(const uint32 index){
		return  static_cast<BalancedTreeNodeT<loadClass,keyClass,loadKey> *>(BalancedTreeNode::Seek(index));
	}

    /**
     * @brief Searches a specified element in this list.
     * @param[in] key a reference to the key to search.
     * @return pointer to the element.
     */
	inline BalancedTreeNodeT<loadClass,keyClass,loadKey> *Search(const keyClass &key,uint32 &index){
		BalancedTreeNodeKey K = loadKey::ToNodeKey(key);
		return static_cast<BalancedTreeNodeT<loadClass,keyClass,loadKey> *>(BalancedTreeNode::Search(K,index) );
	}

    /**
     * @brief Searches the specified key and moves left-right on the basis of the specified offset.
     * @param[in] key a reference to the key to search.
     * @return pointer to the element.
     */
	inline BalancedTreeNodeT<loadClass,keyClass,loadKey> *RelativeSeek(const BalancedTreeNodeKey &key,int32 offset){
		return static_cast<BalancedTreeNodeT<loadClass,keyClass,loadKey> *>(BalancedTreeNode::RelativeSeek(key,offset) );
	}

	/**
	 *  supports only deleteNode and terminateIteration
	 */
	inline IteratorAction Iterate(GenericIterator<loadClass> &iterator,uint32 depth){
		IteratorAction ret;

		// do first smaller subtree
		if (smaller != NULL){
			IteratorAction ia;
			ia = Smaller()->Iterate(iterator,depth+1);
			if (ia){
				switch (ia.ActionCode()){
				case noAction:{

				}break;
				case deleteNode:{
					BalancedTreeNode *extracted;
					BalancedTreeNodeKey dummy;
					if (ExtractAVL(smaller, extracted,dummy,0)){
						delete extracted;
					}
				} break;
				default:{
					// overwrite the whole bitset to clear the not an error code bit
					ret.format_as_integer = ErrorManagement::InvalidOperation;
				}
				}

			} else { // is errorCode
				ret = ia;
			}

		}
		// if no error or termination requests
		// do central node
		// but demand to caller implementing any action
		// this to allow deleting nodes without affecting the structure...
		if (ret){
			IteratorAction ia;
			ret = iterator.Do(GetData(),depth);
		}

		// if there was no termination request or errors so far
		if (ret){
			if (greater != NULL){
				IteratorAction ia;
				ia = Greater()->Iterate(iterator,depth+1);
				if (!ia){
					ret = ia;
				} else {
					switch (ia.ActionCode()){
					case noAction:{

					}break;
					case deleteNode:{
						BalancedTreeNode *extracted;
						BalancedTreeNodeKey dummy;
						if (ExtractAVL(greater, extracted,dummy,0)){
							delete extracted;
						}
					} break;
					default:{
						// overwrite the whole bitset to clear the not an error code bit
						ret = ErrorManagement::InvalidOperation;
					}
					}
				}
			}
		}
		UpdateStatistics();
		return ret;
	}

	/**
	 *
	 */
	loadClass & GetData(){
		return payLoad;
	}
private:

	/**
	 *
	 */
	inline BalancedTreeNodeT<loadClass,keyClass,loadKey> *Smaller(){
		return static_cast<BalancedTreeNodeT<loadClass,keyClass,loadKey> *>(smaller);
	}

	/**
	 *
	 */
	inline BalancedTreeNodeT<loadClass,keyClass,loadKey> *Greater(){
		return static_cast<BalancedTreeNodeT<loadClass,keyClass,loadKey> *>(greater);
	}


	/**
	 * to be subclassed and specialised
	 */
	virtual int8 CompareToKey(BalancedTreeNodeKey K){
		return loadKey::CompareToKey(GetData(),K);
	}

	/**
	 * to be subclassed and specialised
	 */
	virtual int8 CompareToNode(BalancedTreeNode *N){
		if (N== NULL) return -1;
		BalancedTreeNodeT<loadClass,keyClass,loadKey> *node = static_cast<BalancedTreeNodeT<loadClass,keyClass,loadKey> *> (N);
		return loadKey::Compare(GetData(),node->GetData());
	}
	/**
	 *
	 */
	loadClass payLoad;


};



}


#endif /* BALANCEDTREENODET_H_ */
