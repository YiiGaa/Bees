# _BrokerLogger-输出自定义日志

主要功能包括：

- info-输出info log
- error-输出error log
- debug-输出debug log
- console-不采用格式，直接输出info log

模块参数的值说明：

- 如果希望设置的值为固定值，则直接设置对应类型的值即可
- 如果希望从`passParam`中动态获取值作为设置的值，则请以`get##key`作为模块参数的设置值
- 如果想要获取的值在passParam的里层（嵌套json），则请以`>>`定位，如`get##key_1>>key_2`

# ※ info-输出info log

- 组入版本：1.0=2025.01.12

```
{
	"name":"_BrokerLogger",
	"param":{
    	"_action":"info",
		"_message":""
	}
}
```

| key           | 说明                                                    | 是否必要 | 默认值 | 类型   | 组入/更新版本  |
| ------------- | ------------------------------------------------------- | -------- | ------ | ------ | -------------- |
| _action       | 固定为"info"                                            | 是       |        | String | 1.0=2025.01.12 |
| _message      | 打印的log值                                             | 是       |        | String | 1.0=2025.01.12 |
| _isPrintParam | 是否打印moduleParam和passParam，false:不打印，true:打印 | 否       | false  | Bool   | 1.0=2025.01.12 |

模块参数补充说明：

### > _message

- 最终的输出格式为`[时间][info][线程号][所在task][所在位置] log字符串,调用代码位置,moduleParam:{},passParam:{}`


# ※ debug-输出debug log

- 组入版本：1.0=2025.01.12
- Debug log在正式运行时，不会输出日志

```
{
	"name":"_BrokerLogger",
	"param":{
    	"_action":"debug",
		"_message":""
	}
}
```

| key           | 说明                                                    | 是否必要 | 默认值 | 类型   | 组入/更新版本  |
| ------------- | ------------------------------------------------------- | -------- | ------ | ------ | -------------- |
| _action       | 固定为"debug"                                           | 是       |        | String | 1.0=2025.01.12 |
| _message      | 打印的log值                                             | 是       |        | String | 1.0=2025.01.12 |
| _isPrintParam | 是否打印moduleParam和passParam，false:不打印，true:打印 | 否       | false  | Bool   | 1.0=2025.01.12 |

模块参数补充说明：

### > _message

- 最终的输出格式为`[时间][debug][线程号][所在task][所在位置] log字符串,调用代码位置,moduleParam:{},passParam:{}`

# ※ error - 输出error log

- 组入版本：1.0=2025.01.12

```
{
	"name":"_BrokerLogger",
	"param":{
    	"_action":"error",
		"_message":""
	}
}
```

| key           | 说明                                                    | 是否必要 | 默认值 | 类型   | 组入/更新版本  |
| ------------- | ------------------------------------------------------- | -------- | ------ | ------ | -------------- |
| _action       | 固定为"error"                                           | 是       |        | String | 1.0=2025.01.12 |
| _message      | 打印的log值                                             | 是       |        | String | 1.0=2025.01.12 |
| _isPrintParam | 是否打印moduleParam和passParam，false:不打印，true:打印 | 否       | false  | Bool   | 1.0=2025.01.12 |

模块参数补充说明：

### > _message

- 最终的输出格式为`[时间][error][线程号][所在task][所在位置] log字符串,调用代码位置,moduleParam:{},passParam:{}`

# ※ console-不采用格式，直接输出info log

- 组入版本：1.0=2024.04.04

```
{
	"name":"_BrokerLogger",
	"param":{
    	"_action":"console",
		"_message":""
	}
}
```

| key      | 说明            | 是否必要 | 默认值 | 类型   | 组入/更新版本  |
| -------- | --------------- | -------- | ------ | ------ | -------------- |
| _action  | 固定为"console" | 是       |        | String | 1.0=2024.04.04 |
| _message | 打印的log值     | 是       |        | String | 1.0=2024.04.04 |

模块参数补充说明：

### > _message

- 此方式输出的日志不带有上述日志的格式

# ◎ 模块单独测试

在工程根目录，运行shell指令

```
python3 Christmas/Christmas.py ShellExcute/Build#Module _BrokerLogger
```

也可以通过`Christmas插件`运行`ShellExcute>>Build#Module`，并在插件打开的终端中输入`_BrokerLogger`

`sample.cpp`是专门用于单独测试的代码

模块编译后会自动运行，编译的可执行文件生成在`/Build/sample`

# ◎ 更新列表

**1.0=2025.01.12**

- 模块建立