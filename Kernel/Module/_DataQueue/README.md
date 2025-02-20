# _DataQueue-数据队列

主要功能包括：

- create-创建队列
- push-存放数据
- pull-获取数据
- pop-销毁数据
- delete-删除队列

模块参数的值说明：

- 如果希望设置的值为固定值，则直接设置对应类型的值即可
- 如果希望从`passParam`中动态获取值作为设置的值，则请以`get##key`作为模块参数的设置值
- 如果想要获取的值在passParam的里层（嵌套json），则请以`>>`定位，如`get##key_1>>key_2`

# ※ create-创建队列

- 组入版本：1.0=2025.01.12

```
{
	"name":"_DataQueue",
	"param":{
    "_action":"create",
    "_identity":"",
    "_queueMaxLen":10
	}
}
```

| key       | 说明                       | 是否必要 | 默认值     | 类型   | 组入/更新版本  |
| --------- | -------------------------- | -------- | ---------- | ------ | -------------- |
| _action | 固定为"create" | 是       |            | String | 1.0=2025.01.12 |
| _identity | 队列识别，要求唯一            | 是       |            | String | 1.0=2025.01.12 |
| _queueMaxLen | 队列最大长度               | 否       | -1（不限制） | Int    | 1.0=2025.01.12 |

# ※ push-存放数据

- 组入版本：1.0=2025.01.12

```
{
	"name":"_DataQueue",
	"param":{
		"_action":"push",
		"_identity":"",
		"_data":""
	}
}
```

| key          | 说明                                                         | 是否必要 | 默认值   | 类型     | 组入/更新版本  |
| ------------ | ------------------------------------------------------------ | -------- | -------- | -------- | -------------- |
| _action      | 固定为"push"                                                 | 是       |          | String   | 1.0=2025.01.12 |
| _identity    | 队列识别                                                     | 是       |          | String   | 1.0=2025.01.12 |
| _data        | 存储的数据                                                   | 是       |          | 开放类型 | 1.0=2025.01.12 |
| _isFullWait  | 当队列满时，是否等待，true:等待，false:不等待                | 否       | true     | Bool     | 1.0=2025.01.12 |
| _timeout     | 等待超时时间（秒），超时会调用`_timeoutCall`的函数，调用完毕继续等待，如此循环 | 否       | 持续等待 | Int      | 1.0=2025.01.12 |
| _timeoutCall | 等待超时调用的函数，传递当前passParam                        | 否       |          | String   | 1.0=2025.01.12 |

模块参数补充说明：

### > _isFullWait

- 当队列满时，是否等待，true:等待，false:不等待
- 设置为等待`true`，数据会先放入队列，然后进行等待，直到某个数据被销毁
- 设置为不等待`false`，数据会先放入队列，然后强制清理队列的第一个数据

### > _timeoutCall

- 等待超时调用的函数，传递当前passParam
- 调用的函数名称为`Task`的函数，命名格式为`Task名::函数名`，例如`OrderGet::OrderListen`
- 编译整个工程时，会自动登记所有Task中除去`Start`以外的函数，并以`Task名::函数名`的格式登记函数，记录在`Task/Task.hpp`文件中
- 在`sample.cpp`测试代码中，调用的函数需要先通过`TOOLS_CallFunctionMark()`注册

# ※ pull-获取数据

- 组入版本：1.0=2025.01.12
- 先存入的数据会先取出

```
{
	"name":"_DataQueue",
	"param":{
    	"_action":"pull",
    	"_identity":"",
    	"_resultKey":""
	}
}
```

| key          | 说明                                                         | 是否必要 | 默认值   | 类型   | 组入/更新版本  |
| ------------ | ------------------------------------------------------------ | -------- | -------- | ------ | -------------- |
| _action      | 固定为"pop"                                                  | 是       |          | String | 1.0=2025.01.12 |
| _identity    | 队列识别                                                     | 是       |          | String | 1.0=2025.01.12 |
| _resultKey   | 取出的数据存储到passParam的key                               | 是       |          | String | 1.0=2025.01.12 |
| _dataIdKey   | 将数据Id存储到passParam的key                                 | 否       | dataId   | String | 1.0=2025.01.12 |
| _isPop       | 是否立刻销毁数据，true:是，false:否                          | 否       | true     | String | 1.0=2025.01.12 |
| _isEmptyWait | 当队列为空，是否等待，true:等待，false:不等待                | 否       | true     | Bool   | 1.0=2025.01.12 |
| _timeout     | 等待超时时间（秒），超时会调用`_timeoutCall`的函数，调用完毕继续等待，如此循环 | 否       | 持续等待 | Int    | 1.0=2025.01.12 |
| _timeoutCall | 等待超时调用的函数，传递当前passParam                        | 否       |          | String | 1.0=2025.01.12 |

模块参数补充说明：

### > _isPop

- 是否立刻销毁数据，true:是，false:否
- 设置为`true`，会立刻移除队列中的数据
- 设置为`false`，不会从队列中移除数据，仅标记`in use`
- `in use`的数据不会被重复获取，但是会占用队列位置

### > _dataIdKey

- 将数据Id存储到passParam的key
- 数据Id是一个不断增长的`Int`数据，以方便在调用模块的`pop-销毁数据`功能时，移除对应的`in use`数据
- 当`_isPop`设置为`true`时，此设置无效

### > _isFullWait

- 当队列空时，是否等待，true:等待，false:不等待
- 设置为等待`true`，队列为空时，进入等待，直到某个数据被存入
- 设置为不等待`false`，队列为空时，会报错

### > _timeoutCall

- 等待超时调用的函数，传递当前passParam
- 调用的函数名称为`Task`的函数，命名格式为`Task名::函数名`，例如`OrderGet::OrderListen`
- 编译整个工程时，会自动登记所有Task中除去`Start`以外的函数，并以`Task名::函数名`的格式登记函数，记录在`Task/Task.hpp`文件中
- 在`sample.cpp`测试代码中，调用的函数需要先通过`TOOLS_CallFunctionMark()`注册

# ※ pop-销毁数据

- 组入版本：1.0=2025.01.12
- 仅用于销毁`in use`状态的数据

```
{
	"name":"_DataQueue",
	"param":{
    	"_action":"pop",
    	"_identity":""
	}
}
```

| key              | 说明                                        | 是否必要 | 默认值                       | 类型   | 组入/更新版本  |
| ---------------- | ------------------------------------------- | -------- | ---------------------------- | ------ | -------------- |
| _action          | 固定为"pop"                                 | 是       |                              | String | 1.0=2025.01.12 |
| _identity        | 队列识别                                    | 是       |                              | String | 1.0=2025.01.12 |
| _id              | 销毁的数据的id                              | 否       | 销毁第一个`in use`状态的数据 | String | 1.0=2025.01.12 |
| _isNotFoundError | 销毁失败，是否报错，true:报错，false:不报错 | 否       | false                        | Bool   | 1.0=2025.01.12 |

模块参数补充说明：

### > _id

- 销毁的数据的id
- 若不设置，默认销毁第一个`in use`状态的数据

# ※ delete-删除队列

- 组入版本：1.0=2025.01.12
- 自动唤醒所有处于等待的线程，这些等待的线程都会报错

```
{
	"name":"_DataQueue",
	"param":{
    	"_action":"delete",
    	"_identity":""
	}
}
```

| key       | 说明           | 是否必要 | 默认值 | 类型   | 组入/更新版本  |
| --------- | -------------- | -------- | ------ | ------ | -------------- |
| _action   | 固定为"delete" | 是       |        | String | 1.0=2025.01.12 |
| _identity | 队列识别       | 是       |        | String | 1.0=2025.01.12 |

模块参数补充说明：

### > _identity

- 队列识别
- 删除队列并不是立刻完成的，除非当前无任何等待中的线程
- 如果存在等待线程，将会由最后一个唤醒的线程完成队列删除工作

# ◎ 模块单独测试

在工程根目录，运行shell指令

```
python3 Christmas/Christmas.py ShellExcute/Build#Module _BrokerCall
```

也可以通过`Christmas插件`运行`ShellExcute>>Build#Module`，并在插件打开的终端中输入`_BrokerCall`

`sample.cpp`是专门用于单独测试的代码

模块编译后会自动运行，编译的可执行文件生成在`/Build/sample`

# ◎ 更新列表

**1.0=2025.01.12**

- 模块建立