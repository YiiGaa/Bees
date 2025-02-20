# _BrokerThread-线程操作

主要功能包括：

- create-创建线程
- wait-等待某个线程结束
- wait all-等待所有线程结束

模块参数的值说明：

- 如果希望设置的值为固定值，则直接设置对应类型的值即可
- 如果希望从`passParam`中动态获取值作为设置的值，则请以`get##key`作为模块参数的设置值
- 如果想要获取的值在passParam的里层（嵌套json），则请以`>>`定位，如`get##key_1>>key_2`

# ※ create-创建线程

- 组入版本：1.0=2025.01.12
- 创建线程实际上是启动一个线程，并调用`Task`的`Start()`函数

```
{
	"name":"_BrokerThread",
	"param":{
		"_action":"create",
		"_identity":"",
		"_call":""
	}
}
```

| key       | 说明                                       | 是否必要 | 默认值         | 类型   | 组入/更新版本  |
| --------- | ------------------------------------------ | -------- | -------------- | ------ | -------------- |
| _action   | 固定为"create"                             | 是       |                | String | 1.0=2025.01.12 |
| _identity | 识别key，要求唯一，一般采用`_call`相同的值 | 是       |                | String | 1.0=2025.01.12 |
| _call     | 调用的`Task`名称                           | 否       | 传递空json对象 | String | 1.0=2025.01.12 |
| _param    | 调用函数时，传递的数据池                   | 否       | 整个passParam  | Object | 1.0=2025.01.12 |

模块参数补充说明：

### > _call

- 调用的`Task`名称
- 创建线程实际上是启动一个线程，并调用该`Task`的`Start`函数
- 编译整个工程时，会自动登记所有含有`Start`函数的`Task`，记录在`Task/Task.hpp`文件中，若去除了`Task`中的`Start`函数，则不会被自动登记
- 在`sample.cpp`测试代码中，需要通过`TOOLS_CallThreadMark()`注册

### > _param

- 调用函数时，传递的数据池
- 启动线程前，会复制一个新副本，不会影响到本线程的数据池

# ※ wait-等待某个线程结束

- 组入版本：1.0=2025.01.12
- 挂起当前线程，直到某个线程结束

```
{
	"name":"_BrokerThread",
	"param":{
		"_action":"wait",
		"_identity":""
	}
}
```

| key       | 说明                    | 是否必要 | 默认值 | 类型   | 组入/更新版本  |
| --------- | ----------------------- | -------- | ------ | ------ | -------------- |
| _action   | 固定为"wait"            | 是       |        | String | 1.0=2025.01.12 |
| _identity | 识别key，对应创建时的id | 是       |        | String | 1.0=2025.01.12 |

# ※ wait all-等待所有线程结束

- 组入版本：1.0=2025.01.12
- 挂起当前线程，直到所有线程结束


```
{
	"name":"_BrokerThread",
	"param":{
		"_action":"wait all"
	}
}
```

| key    | 说明                        | 是否必要 | 默认值 | 类型   | 组入/更新版本  |
| ------ | --------------------------- | -------- | ------ | ------ | -------------- |
| _action | 固定为"wait all" | 是       |        | String | 1.0=2025.01.12 |

# ◎ 模块单独测试

在工程根目录，运行shell指令

```
python3 Christmas/Christmas.py ShellExcute/Build#Module _BrokerThread
```

也可以通过`Christmas插件`运行`ShellExcute>>Build#Module`，并在插件打开的终端中输入`_BrokerThread`

`sample.cpp`是专门用于单独测试的代码

模块编译后会自动运行，编译的可执行文件生成在`/Build/sample`

# ◎ 更新列表

**1.0=2025.01.12**

- 模块建立