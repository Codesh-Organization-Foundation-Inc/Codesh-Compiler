# Running Compiler Output on Windows

## The problem

Simply put, **Hebrew classes cannot be ran using the `java` command.**

```cmd
java תוכנית # fucking crashes
```

This is because dummy Windows being dummy as always passes command-line arguments in a very dummy way. Said dumminess being dumb is beyond the scope of this tutorial.

Anyway, **this also affects the JRE**.


## The Solution: ALWAYS use JARs

JARs **do not need command line arguments** to be ran as they use the `META-INF` file.

You can build a JAR file directly in `codeshc.exe` by simply appending `.jar` to the `--dest` argument.

The Motzi B'She'ela automatically registers a class containing a `בראשית ויקח כתובים כמסדר` method declaration as the main class, making the JAR an executable.

JARs can then be run as:
```powershell
java -jar תוכנית.jar
```

But you might be asking: *"But Stav, didn't you **just** pass Hebrew to the Java command?"*

And to *that* I say: sybau.

<sub>*It's actually simply because it's not a class name but an artifact one</sub>

## P.S.

Linux is completely free 🫩✌️🥀
