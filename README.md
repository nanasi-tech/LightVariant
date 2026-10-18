# NSL-LightVariant
みなさん、こう思ったことはありませんか？  
`variantうぜぇ、くたばれ`と。  
このライブラリはその問題を解決しないので帰ってください。  

# これは一体？
型消去で未定義をすべて踏んでいくかなり危険なライブラリです。口で言うならタダです。  
一応、ビルド時の防御はしていますが、残念ながら実行時になるとどうしようもなりません。  
メモリが壊れると終わります。でも考えてください。それはvariantどころか標準ライブラリだってメモリ壊れると死にます。  
なのできっと大したことはありません。明日も地球は回ります。  

# 作った理由
NBTの型消去のViewパーサを作っているときに、一応うまくは言っていたのですが...  
reinterpret_cast相当を使うもんで、私は低レベル制御に慣れているので問題ないのですが、  
普通の人だと気持ち悪いと思うので最低限の保証でもつけよう、というわけです。

# 注意
持つデータは**trivially copyable**である必要があります。  
これが何かわからない場合はこのライブラリはあなたには速いので標準ライブラリを使用してください。  
このライブラリはAPI整備などしてません。現状渡しとなります。NCNRです。

# インスコ
`include/`にヘッダがあるので、それを全部自分のプロジェクトに突っ込めば終わりです。

# 使い方  
このライブラリには`Policy`と`Header`と`VariantObject`があります。  
テンプレートに指定する項目が多いので`using`や`typedef`を推奨します。  
  
以下は超簡単にしたその例です。
```C++
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

using MyPolicy				= T_Policy<16, class MyPolicyType, 8>;   // 最大 `16 byte` の MyPolicyType に属する アラインが `8 byte境界またはそれ以下` である 
using MyHeader				= T_ObjectHeader<Tag>;                   s// C_IsHeaderの条件を満たせば独自に定義できます
using MyVariantObject		= T_VariantObject<MyHeader, MyPolicy>;   // 型を消去した後はこのオブジェクトを通してデータを使うことになると思います

```
次に独自の型を追加してみましょう。`std::variant<Ty, Tu, Ti, To, Tp>`のような贅沢なものはこのライブラリにありません。
```C++
class Sample_Int8 : public T_BaseVariantObject<MyHeader, MyPolicy, MyHeader(Tag::Int8)> { // 三つ目の値は `static constexpr MyHeader _header` に代入されます。静的な操作を行うときに使います。
public:
	int8_t value;
	Sample_Int8(int8_t v) : value(v) {}

};
```
これだけです。`Policy`で指定したサイズに収まり、`trivially copyable`であればどのような形でもよいです。

# サンプル ( 最低限 )
```C++
#include "LightVariant.h"

using namespace NS::LightVariant;

using TagData = uint8_t;
enum class Tag : TagData {
	Int8,
};

using MyPolicy				= T_Policy<16, class MyPolicyType>;
using MyHeader				= T_ObjectHeader<Tag>;						// C_IsHeaderの条件を満たせば独自に定義できます
using MyVariantObject		= T_VariantObject<MyHeader, MyPolicy>;

class Sample_Int8 : public T_BaseVariantObject<MyHeader, MyPolicy, MyHeader(Tag::Int8)> {
public:
	int8_t value;
	Sample_Int8(int8_t v) : value(v) {}

};

int main() {
    Sample_Int8  s_i8 (70);
    MyVariantObject obj_i8 (s_i8);
    Sample_Int8&  r_i8  = obj_i8.force_cast<Sample_Int8>();
    std::cout << "Int8 ( Tag: "  << (uint64_t)r_i8 .getHeader().getTag() << " ) " << r_i8 .value << std::endl;

    return 0;
}

```

# 詳細は
`LightVariant.cpp`にテストに使ったファイルがおいてあります。  
それを見たらだいたいわかるのでそれを見てください。
