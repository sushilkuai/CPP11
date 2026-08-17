#include <string>
#include <tuple>
#include <iostream>
#include <vector>



//C++98 had std::pair for two values. For three or more, you had to define a struct every time:
// C++98 — need a struct just to return three values
struct QueryResult {
    bool        success;
    int         rowCount;
    std::string errorMsg;
};

QueryResult runQuery() {
    //do processing and create a struct object, then return
    return QueryResult{ 0,3,"t" };
}

//----------------------------------------------------
//std::tuple is a generalization of std::pair to any number of elements of any types:

//std::tuple<bool, int, std::string> runQuery1();

void basicDemo()
{
    //create with explicit type
    std::tuple<int, double, std::string> t1(42, 3.14, "hello");

    //create with make_tuple
    auto t2 = std::make_tuple(42, 3.14, "hello");
    std::cout<< "t1 and t2 are same ? -"  <<  std::is_same<decltype(t1), decltype(t2)>::value<<"-\n";
    //access by compiletime index
    std::cout << std::get<0>(t1);
    std::cout << std::get<2>(t2)<<"\n"<<std::endl;
    //also modify
    std::get<1>(t1) = 100;

    // Size and element type at COMPILE time
    std::cout << "Elements: "
        << std::tuple_size<decltype(t2)>::value << "\n";  // 3
    std::cout << "Element 1 is double: "<<
    std::is_same<std::tuple_element<1, decltype(t2)>::type, double>::value<<std::endl;
    //compile time check
    static_assert(std::is_same<std::tuple_element<1, decltype(t2)>::type, double>::value == 1, "test is passed");


 
}

// returing multiple values
std::tuple<bool, int, std::string> QueryDatabase(const std::string& sql)
{
    if(sql.empty())
    {
        return std::make_tuple(false, 0, "empty");
    }
    
    return std::make_tuple(true, 1, "OK");
}

void multiReturnDemo()
{
    auto result = QueryDatabase("select all from orders");
    std::cout << "message from result=" << std::get<2>(result) << std::endl;

    //bind the result into names variables
    bool res;
    int rows;
    std::string msg;
    //std::tie creates a tuple of lvalue references to existing variables, 
    // so assigning a tuple to it unpacks directly into those variables :
    std::tie(res, rows, msg) = QueryDatabase("my query");

    //ignore some values
    std::tie(res,std::ignore, msg) = QueryDatabase("my query");


    //touple cat
    auto coords = std::make_tuple(10, 20, 30);
    auto coords2 = std::make_tuple(40, 50);
    auto co3 = std::tuple_cat(coords, coords2);
}

void checkTuple()
{
    basicDemo();
    multiReturnDemo();
}


