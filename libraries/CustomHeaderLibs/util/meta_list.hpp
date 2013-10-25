// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    21.07.2013 11:54:08 EDT
// File:    meta_list.hpp

/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ or COPYING for more details. */

#ifndef __META_LIST_HEADER
#define __META_LIST_HEADER

namespace util {
    //------------------- mtp list -------------------
    template<typename stored_type, typename next_list_item>
    struct list_item {
        typedef stored_type type;
        typedef next_list_item next;
    };

    struct list_end {};  //is used to mark the end of a list

    //------------------- remove void -------------------
    template<typename list>
    struct remove_void {
        typedef list type;
    };

    template<
            typename stored_type
          , typename next_list_item
          > 
    struct remove_void<list_item<stored_type, next_list_item> > {
        typedef list_item< stored_type, typename remove_void<next_list_item>::type> type;
    };

    template<typename next_list_item>
    struct remove_void<list_item<void, next_list_item> > {
        typedef typename remove_void<next_list_item>::type type;
    };

    //------------------- make_list -------------------
    template<
          typename _0  = void
        , typename _1  = void
        , typename _2  = void
        , typename _3  = void
        , typename _4  = void
        , typename _5  = void
        , typename _6  = void
        , typename _7  = void
        , typename _8  = void
        , typename _9  = void
        , typename _10  = void
        , typename _11  = void
        , typename _12  = void
        , typename _13  = void
        , typename _14  = void
        , typename _15  = void
        , typename _16  = void
        , typename _17  = void
        , typename _18  = void
        , typename _19  = void
        , typename _20  = void
        , typename _21  = void
        >
    struct make_list {
        typedef typename remove_void<
                list_item<_0, 
                 list_item<_1, 
                  list_item<_2, 
                   list_item<_3, 
                    list_item<_4, 
                     list_item<_5, 
                      list_item<_6, 
                       list_item<_7,
                        list_item<_8, 
                         list_item<_9, 
                          list_item<_10, 
                           list_item<_11, 
                            list_item<_12, 
                             list_item<_13, 
                              list_item<_14, 
                               list_item<_15, 
                                list_item<_16, 
                                 list_item<_17, 
                                  list_item<_18, 
                                   list_item<_19, 
                                    list_item<_20, 
                                     list_item<_21, 
                          list_end
                 > > > > >  > > > > >
                 > > > > >  > > > > > 
                 > >
                >::type type;
    };
    //------------------- concat two lists -------------------
    template<typename list1, typename list2> struct concatinate_lists {
        typedef list_item< typename list1::type, typename concatinate_lists<typename list1::next, list2>::type> type;
    };

    template<typename list2> struct concatinate_lists<list_end, list2> {
        typedef list2 type;
    };

    //------------------- unique list walker -------------------
    //walks through the list and eliminates target
    template<typename target, typename list>
    struct unique_list_walker {
        typedef list_item< 
            typename list::type
          , typename unique_list_walker<target, typename list::next>::type 
        > type;
    };

    template<typename target, typename list>
    struct unique_list_walker<target, list_item<target, list> > {
        typedef typename unique_list_walker<target, list>::type type;
    };

    template<typename target>
    struct unique_list_walker<target, list_end>{
        typedef list_end type;
    };
    //------------------- unique list -------------------
    template<typename list> 
    struct unique_list {
        typedef list_item<
              typename list::type
            , typename unique_list<
                  typename unique_list_walker<
                        typename list::type
                      , typename list::next
                  >::type
              >::type
        > type;
    };

    template<>
    struct unique_list<list_end> {
        typedef list_end type;
    };
    //------------------- last_item -------------------
    template<typename list>
    struct last_item {
        typedef
            typename last_item<typename list::next>::type
        type;
    };
    template<typename item>
    struct last_item<list_item<item, list_end> > {
        typedef item type;
    };


    //------------------- remove_last -------------------
    template<typename list>
    struct remove_last {
        typedef
            list_item<typename list::type
                    , typename remove_last<typename list::next>::type
            >
        type;
    };
    template<typename item>
    struct remove_last<list_item<item, list_end> > {
        typedef list_end type;
    };

    //------------------- invert list -------------------
    template<typename list>
    struct invert_list {
        typedef 
            list_item<typename last_item<list>::type 
                             , typename invert_list<typename remove_last<list>::type>::type 
            >
        type;
    };
    template<typename item>
    struct invert_list<list_item<item, list_end> > {
        typedef list_item<item, list_end> type;
    };
}//end namespace util

#endif //__META_LIST_HEADER
