#ifndef ORDERINFORMATIONTEST_H
#define ORDERINFORMATIONTEST_H
#include <orderinformation.h>
#include "gtest/gtest.h"

TEST(OrderInformation, ConstructorTest) {
    OrderInformation oi;
    EXPECT_EQ("", oi.GetOrderID().toStdString());
    EXPECT_EQ("", oi.GetCustomerName().toStdString());
    EXPECT_EQ("", oi.GetOrderPriority().toStdString());
}

#endif // ORDERINFORMATIONTEST_H
