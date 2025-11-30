using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;
using System.Linq;

abstract class AbstractBuffer
{
    public int Capacity { get; }
    protected Queue<int> buffer;

    public AbstractBuffer(int capacity)
    {
        if (capacity <= 0) throw new Exception("Invalid buffer capacity");
        Capacity = capacity;
        buffer = new Queue<int>();
    }

    public abstract void Produce(int producerId, int value);
    public abstract int Consume();
    public abstract int GetCount();
}

class Producer
{
    public int Id { get; }
    public int ItemCount { get; }
    private AbstractBuffer buffer;
    private static Random random = new Random();

    public Producer(int id, int itemCount, AbstractBuffer buffer)
    {
        Id = id;
        ItemCount = itemCount;
        this.buffer = buffer;
    }

    public void Run()
    {
        Console.WriteLine($"Producer {Id} STARTED; will produce {ItemCount} items");

        for (int i = 0; i < ItemCount; i++)
        {
            int value = Id * 1000 + i;
            buffer.Produce(Id, value);
            Console.WriteLine($"Producer {Id} produced value {value}");
            Thread.Sleep(random.Next(100, 500));
        }

        Console.WriteLine($"Producer {Id} FINISHED");
    }
}

class Consumer
{
    private AbstractBuffer buffer;
    private int totalItemsToConsume;
    private static Random random = new Random();

    public Consumer(AbstractBuffer buffer, int totalItemsToConsume)
    {
        this.buffer = buffer;
        this.totalItemsToConsume = totalItemsToConsume;
    }

    public void Run()
    {
        Console.WriteLine($"Consumer STARTED; will consume {totalItemsToConsume} items");

        for (int i = 0; i < totalItemsToConsume; i++)
        {
            int value = buffer.Consume();
            Console.WriteLine($"Consumer consumed value {value}");
            Thread.Sleep(random.Next(100, 300));
        }

        Console.WriteLine($"Consumer FINISHED");
    }
}

class BufferSemaphore : AbstractBuffer
{
    private Mutex mutex = new Mutex();
    private SemaphoreSlim emptySemaphore;
    private SemaphoreSlim fullSemaphore;

    public BufferSemaphore(int capacity) : base(capacity)
    {
        emptySemaphore = new SemaphoreSlim(capacity, capacity);
        fullSemaphore = new SemaphoreSlim(0, capacity);
    }

    public override void Produce(int producerId, int value)
    {
        emptySemaphore.Wait();

        mutex.WaitOne();
        buffer.Enqueue(value);
        Console.WriteLine($"  [Buffer] Producer {producerId} added to buffer. Buffer size: {buffer.Count}/{Capacity}");
        mutex.ReleaseMutex();

        fullSemaphore.Release();
    }

    public override int Consume()
    {
        fullSemaphore.Wait();

        mutex.WaitOne();
        int value = buffer.Dequeue();
        Console.WriteLine($"  [Buffer] Consumer removed from buffer. Buffer size: {buffer.Count}/{Capacity}");
        mutex.ReleaseMutex();

        emptySemaphore.Release();

        return value;
    }

    public override int GetCount()
    {
        mutex.WaitOne();
        int count = buffer.Count;
        mutex.ReleaseMutex();
        return count;
    }
}

class BufferMonitor : AbstractBuffer
{
    private object lockObject;

    public BufferMonitor(int capacity) : base(capacity)
    {
        lockObject = new object();
    }

    public override void Produce(int producerId, int value)
    {
        lock (lockObject)
        {
            while (buffer.Count >= Capacity)
            {
                Console.WriteLine($"  [Buffer] Producer {producerId} waiting - buffer full");
                Monitor.Wait(lockObject);
            }

            buffer.Enqueue(value);
            Console.WriteLine($"  [Buffer] Producer {producerId} added to buffer. Buffer size: {buffer.Count}/{Capacity}");

            Monitor.PulseAll(lockObject);
        }
    }

    public override int Consume()
    {
        lock (lockObject)
        {
            while (buffer.Count == 0)
            {
                Console.WriteLine($"  [Buffer] Consumer waiting - buffer empty");
                Monitor.Wait(lockObject);
            }

            int value = buffer.Dequeue();
            Console.WriteLine($"  [Buffer] Consumer removed from buffer. Buffer size: {buffer.Count}/{Capacity}");

            Monitor.PulseAll(lockObject);

            return value;
        }
    }

    public override int GetCount()
    {
        lock (lockObject)
        {
            return buffer.Count;
        }
    }
}

class Program
{
    static void Main(string[] args)
    {
        Console.WriteLine("=== PRODUCER-CONSUMER PROBLEM ===\n");

        int producerCount = 3;
        int bufferCapacity = 5;
        int[] itemsPerProducer = { 4, 3, 5 };
        int totalItems = itemsPerProducer.Sum();

        Console.WriteLine($"Configuration:");
        Console.WriteLine($"  Producers: {producerCount}");
        Console.WriteLine($"  Buffer capacity: {bufferCapacity}");
        Console.WriteLine($"  Items per producer: {string.Join(", ", itemsPerProducer)}");
        Console.WriteLine($"  Total items: {totalItems}\n");

        //Console.WriteLine("\n=== Using SEMAPHORE implementation ===\n");
        // var buffer = new BufferSemaphore(bufferCapacity);
        Console.WriteLine("\n=== Using MONITOR implementation ===\n");
        var buffer = new BufferMonitor(bufferCapacity);

        Console.WriteLine("Start\n");

        var producerTasks = new Task[producerCount];
        for (int i = 0; i < producerCount; i++)
        {
            var producer = new Producer(i + 1, itemsPerProducer[i], buffer);
            producerTasks[i] = new Task(producer.Run);
        }

        var consumer = new Consumer(buffer, totalItems);
        var consumerTask = new Task(consumer.Run);

        foreach (var task in producerTasks)
        {
            task.Start();
        }
        consumerTask.Start();

        Task.WaitAll(producerTasks.Concat([consumerTask]).ToArray());

        Console.WriteLine($"\nEnd");
        Console.WriteLine($"Final buffer size: {buffer.GetCount()}");
    }
}
