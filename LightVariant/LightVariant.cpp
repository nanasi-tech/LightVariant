
/*

NS@Tech @2026
LightVariant ( DEV TEST )

型消去をします。中に識別IDを持ちます。
trivially copyableが条件です。

勘違いしないでほしいのが、このライブラリはコンパイル時の型は強く保障する一方で
ランタイムでのメモリ破壊などでは滅法弱く、またそれに依存しているため
型保障がある程度あるとはいえ過信してはいけません。

Compoundで型消去して参照を効率よく保持するというものを開発していたのですが
全部reinterpret_castだったので
せめてもの何か保障してくれるものがほしいな、とそういう風に思って徒然なるままに作ったものです
参照を保持するとかには向いてるんじゃないですかね、stringとかは持てませんけど

*/

#include <iostream>
#include "include/NS/LightVariant/LightVariant.h"
using namespace NS::LightVariant;

using TagData = uint8_t;
enum class Tag : TagData {
	Int8,
	Int16,
	Int32,
	Int64,
	Float, 
	Double,
	Array,
};

using MyPolicy				= T_Policy<16, class MyPolicyType, 8>;		// 最大 `16 byte` の MyPolicyType に属する アラインが `8 byte境界またはそれ以下` である 
using MyHeader				= T_ObjectHeader<Tag>;						// C_IsHeaderの条件を満たせば独自に定義できます
using MyVariantObject		= T_VariantObject<MyHeader, MyPolicy>;

class Sample_Int8 : public T_BaseVariantObject<MyHeader, MyPolicy, MyHeader(Tag::Int8)> {
public:
	int8_t value;
	Sample_Int8(int8_t v) : value(v) {}

};

class Sample_Int16 : public T_BaseVariantObject<MyHeader, MyPolicy, MyHeader(Tag::Int16)> {
public:
	int16_t value;
	Sample_Int16(int16_t v) : value(v) {}

};

class Sample_Int32 : public T_BaseVariantObject<MyHeader, MyPolicy, MyHeader(Tag::Int32)> {
public:
	int32_t value;
	Sample_Int32(int32_t v) : value(v) {}

};

class Sample_Int64 : public T_BaseVariantObject<MyHeader, MyPolicy, MyHeader(Tag::Int64)> {
public:
	int64_t value;
	Sample_Int64(int64_t v) : value(v) {}

};

class Sample_Array : public T_BaseVariantObject<MyHeader, MyPolicy, MyHeader(Tag::Array)> {
public:
	bool need_swap = false;
	char __pad_0x02[0x02];
	int32_t length = 0;
	uint8_t* begin = nullptr;

	Sample_Array(uint8_t* _begin, int32_t _length) : begin(_begin), length(_length) {}
};

int main() {
	std::cout << "Hello World" << std::endl;

	uint8_t _for_sample_array[] = {
		5, 10, 85, 250, 103,
	};

	Sample_Int8  s_i8 (70);
	Sample_Int32 s_i32(5800);
	Sample_Int64 s_i64(-4870);
	Sample_Array s_arr(_for_sample_array, sizeof(_for_sample_array));

	MyVariantObject obj_i8 (s_i8);		// ポインタキャストで直接変換するのもアリですが、オブジェクト外のメモリもmemcpyする可能性があるので、作者としてはこのようにコピーすることをおすすめします。
	MyVariantObject obj_i32(s_i32);		// 一方、使用者としては、私はポインタキャストをします。
	MyVariantObject obj_i64(s_i64);
	MyVariantObject obj_arr(s_arr);

	Sample_Int8&  r_i8  = obj_i8.force_cast<Sample_Int8>();
	Sample_Int32& r_i32 = obj_i32.force_cast<Sample_Int32>();
	Sample_Int64& r_i64 = obj_i64.force_cast<Sample_Int64>();
	Sample_Array& r_arr = obj_arr.force_cast<Sample_Array>();

	std::cout << "Int8 ( Tag: "  << (uint64_t)r_i8 .getHeader().getTag() << " ) " << r_i8 .value << std::endl;
	std::cout << "Int32 ( Tag: " << (uint64_t)r_i32.getHeader().getTag() << " ) " << r_i32.value << std::endl;
	std::cout << "Int64 ( Tag: " << (uint64_t)r_i64.getHeader().getTag() << " ) " << r_i64.value << std::endl;
	std::cout << "Array ( Tag: " << (uint64_t)r_arr.getHeader().getTag() << " ) " << "Length: " << r_arr.length << std::endl;

	std::cout << "\ntry_cast test ( 0 = >> KEIKAKU DOORI <<, 1 = we have some bug )" << std::endl;
	std::cout << "Int8 to Int64 " << (bool)obj_i8.try_cast<Sample_Int64>() << std::endl;
	std::cout << "Arry to Int32 " << (bool)obj_arr.try_cast<Sample_Int32>() << std::endl;

}

