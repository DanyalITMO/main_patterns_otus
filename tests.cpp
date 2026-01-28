#include <gtest/gtest.h>
#include "solve.h"


//3. Написать тест, который проверяет, что для уравнения x^2+1 = 0 корней нет (возвращается пустой массив)
TEST(quadratic_equation, first) {
  ASSERT_EQ(solve(1,0,1).size(), 0);
}
//5. Написать тест, который проверяет, что для уравнения x^2-1 = 0 есть два корня кратности 1 (x1=1, x2=-1)
TEST(quadratic_equation, second) {
  auto root = solve(1,0,-1);
  ASSERT_EQ(root.size(), 2);
  ASSERT_DOUBLE_EQ(root[0], 1);
  ASSERT_DOUBLE_EQ(root[1], -1);
}
//7. Написать тест, который проверяет, что для уравнения x^2+2x+1 = 0 есть один корень кратности 2 (x1= x2 = -1).
TEST(quadratic_equation, third) {
  auto root = solve(1,2,1);
  ASSERT_EQ(root.size(), 1);
  ASSERT_DOUBLE_EQ(root[0], -1);
}

// 9. Написать тест, который проверяет, что коэффициент a не может быть равен 0.
// В этом случае solve выбрасывает исключение.
//Примечание.Учесть, что a имеет тип double и сравнивать с 0 через == нельзя.
TEST(quadratic_equation, fourth) {
  ASSERT_THROW(solve(0,2,1), std::invalid_argument);
}

//11. учетом того, что дискриминант тоже нельзя сравнивать с 0 через знак равенства, подобрать такие коэффициенты
//квадратного уравнения для случая одного корня кратности два, чтобы дискриминант был отличный от нуля, но меньше
//заданного эпсилон. Эти коэффициенты должны заменить коэффициенты в тесте из п. 7.

TEST(quadratic_equation, e_test) {
  auto root = solve(1.000000000001,2.000000000014,1.000000000001);
  ASSERT_EQ(root.size(), 1);
  ASSERT_NEAR(root[0], -1.0, e);
  // ASSERT_DOUBLE_EQ(root[0], -1.0);//слишком высокая точность для такого порядка чисел
}

//13. Посмотреть какие еще значения могут принимать числа типа double, кроме числовых и написать тест с их использованием
//на все коэффициенты. solve должен выбрасывать исключение.
TEST(quadratic_equation, 8test) {
  ASSERT_THROW(solve(std::numeric_limits<double>::quiet_NaN(),2,1), std::invalid_argument);
  ASSERT_THROW(solve(2,std::numeric_limits<double>::quiet_NaN(),1), std::invalid_argument);
  ASSERT_THROW(solve(1,1, std::numeric_limits<double>::quiet_NaN()), std::invalid_argument);


  ASSERT_THROW(solve(std::numeric_limits<double>::infinity(),2,1), std::invalid_argument);
  ASSERT_THROW(solve(2,std::numeric_limits<double>::infinity(),1), std::invalid_argument);
  ASSERT_THROW(solve(1,1, std::numeric_limits<double>::infinity()), std::invalid_argument);
}


