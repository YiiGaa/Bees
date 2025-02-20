# _ServerHttp-Http/Https请求

主要功能包括：

- Http/Https请求

模块参数的值说明：

- 如果希望设置的值为固定值，则直接设置对应类型的值即可
- 如果希望从`passParam`中动态获取值作为设置的值，则请以`get##key`作为模块参数的设置值
- 如果想要获取的值在passParam的里层（嵌套json），则请以`>>`定位，如`get##key_1>>key_2`

# ※ 发送Http/Https请求

- 组入版本：1.0=2025.01.12
- 请求返回的状态码需为2XX，否则会认为请求不正常，报错

```
{
	"name":"_ServerHttp",
	"param":{
    "_url":"",
    "_param":"",
    "_resultKey":""
	}
}
```

| key       | 说明                     | 是否必要 | 默认值                | 类型   | 组入/更新版本  |
| --------- | ------------------------ | -------- | --------------------- | ------ | -------------- |
| _url   | 请求的URL | 是     |  | String | 1.0=2025.01.12 |
| _method | 请求方式，POST/DELETE/PUT/PATCH/GET | 是       | POST | String | 1.0=2025.01.12 |
| _param | 请求体 | 否       |  | Object | 1.0=2025.01.12 |
| _paramType | 请求体类型（Content-type） | 否       | application/json | String | 1.0=2024.03.16 |
| _header | 请求header的设置 | 否 |  | Object | 1.0=2024.04.27 |
| _resultKey | 请求返回的数据存储到passParam的key | 是 |  | String | 1.0=2024.04.27 |
| _isVerifySSL | 当请求为https时，是否验证ssl证书，false:不认证，true:认证 | 否 | false | Bool | 1.0=2024.04.27 |

模块参数补充说明：

### > _url

- 请求的URL
- 请设置完整的URL，如https://stoprefactoring.com/test/test
- 当请求方式为GET时，建议将参数设置在`_param`中

### > _method

- 请求方式，支持5中请求方式，POST/DELETE/PUT/PATCH/GET
- 需要严格按照大写单词填入，否则报错

### > _param

- 请求体
- 建议使用Json 对象`{}`
- 实际上，这个参数是一个开放类型，可以传递字符串、数字、bool、数组`[]`等
- 当请求方式为 `GET`（一些服务可能会拒绝GET的请求体），且当前参数为Json 对象`{}`，则自动拼接到_url中，如`key1=value1&key2=value2`
- 自动将请求体拼接到_url时，会自动对key、value进行URL编码

### > _isVerifySSL

- 当请求为https时，是否验证ssl证书，false:不认证，true:认证
- 验证的是服务端的SSL证书，若是测试时，或者采用自签证书，则需要设置为`false`，否则会报错

# ◎ 配置说明

模块配置在`_ServerHttp/Once.Config`中设置，通过Christmas运行应用时，会自动拼接到`/Kernel/Common/Config/Config.hpp`

| key                                       | 说明                                                 | 默认值 | 类型   | 组入/更新版本  |
| ----------------------------------------- | ---------------------------------------------------- | --------  | ------ | -------------- |
| CONFIG__ServerHttp_TimeoutConnect | 连接超时时间（秒） | 10 | Int | 1.0=2025.01.12 |
| CONFIG__ServerHttp_TimeoutRead    | 读超时时间（秒） | 5 | Int | 1.0=2025.01.12 |
| CONFIG__ServerHttp_TimeoutWrite   | 写超时时间（秒） | 5 | Int | 1.0=2025.01.12 |

# ◎ 模块单独测试

在工程根目录，运行shell指令

```
python3 Christmas/Christmas.py ShellExcute/Build#Module _ServerHttp
```

也可以通过`Christmas插件`运行`ShellExcute>>Build#Module`，并在插件打开的终端中输入`_ServerHttp`

`sample.cpp`是专门用于单独测试的代码

模块编译后会自动运行，编译的可执行文件生成在`/Build/sample`

# ◎ 更新列表

**1.0=2025.01.12**

- 模块建立