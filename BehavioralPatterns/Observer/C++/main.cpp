/**
 * Observer Design Pattern
 *
 * Intent: Lets you define a subscription mechanism to notify multiple objects
 * about any events that happen to the object they're observing.
 *
 * Note that there's a lot of different terms with similar meaning associated
 * with this pattern. Just remember that the Subject is also called the
 * Publisher and the Observer is often called the Subscriber and vice versa.
 * Also the verbs "observe", "listen" or "track" usually mean the same thing.
 */
#include <iostream>
#include <list>

class IObserver {
public:
    virtual ~IObserver(){};
    virtual void Update(const std::string &message_from_subject) = 0;
};

class ISubject
{
public:
    virtual ~ISubject(){};
    virtual void Attach(IObserver *observer) = 0;
    virtual void Detach(IObserver *observer) = 0;
    virtual void Notify() = 0;
};

/**
 * The Subject owns some important state and notifies observers when the state
 * changes.
 */
class Subject : public ISubject
{
public:
    virtual ~Subject() {
        std::cout << "Goodbye, I was the Subject.\n";
    }
    /**
   * The subscription management methods.
   */
    void Attach(IObserver *observer) override {
        list_observer_.push_back(observer);
    }
    void Detach(IObserver *obserber) override {
        list_observer_.remove(obserber);
    }
    void Notify() override {
        HowManyObserver();
        for (IObserver *observer : list_observer_)
        {
            observer->Update(message_);
        }      
    }

    void CreateMessage(std::string message = "empty"){
        this->message_ = message;
        Notify();
    }
    void HowManyObserver(){
        std::cout << "There are " << list_observer_.size() << " observers in the list.\n";
    }
    /**
   * Usually, the subscription logic is only a fraction of what a Subject can
   * really do. Subjects commonly hold some important business logic, that
   * triggers a notification method whenever something important is about to
   * happen (or after it).
   */
    void SomeBusinessLogic() {
        this->message_ = "change message message";
        Notify();
        std::cout << "I'm about to do some thing important\n";
    }
private:
    std::list<IObserver *> list_observer_;
    std::string message_;
};

class Observer : public IObserver
{
public:
    Observer(Subject &subject) : subject_(subject){
        this->subject_.Attach(this);
        std::cout << "Hi, I'm the Observer \"" << ++Observer::static_number_ << "\".\n";
        this->number_ = Observer::static_number_;
    }
    virtual ~Observer(){
        std::cout << "Goodbye, I was the Observer \"" << this->number_ << "\".\n";
    }
    void Update(const std::string &message_from_subject) override{
        message_from_subject_ = message_from_subject;
        std::cout << "Observer \"" << this->number_ << "\": a new message is available --> " << this->message_from_subject_ << "\n";
    }
    void RemoveMeFromList(){
        subject_.Detach(this);
        std::cout << "Observer \"" << this->number_ << "\" removed from the list.\n";
    }
private:
    std::string message_from_subject_;
    Subject &subject_;
    static int static_number_;
    int number_;
};
int Observer::static_number_ = 0;

int main()
{
    Subject *subject = new Subject();
    Observer *observer1 = new Observer(*subject);
    Observer *observer2 = new Observer(*subject);
    Observer *observer3 = new Observer(*subject);
    Observer *observer4;
    Observer *observer5;

    subject->CreateMessage("hello world");
    observer3->RemoveMeFromList();

    subject->CreateMessage("The weather is hot today! :p");
    observer4 = new Observer(*subject);

    observer2->RemoveMeFromList();
    observer5 = new Observer(*subject);

    subject->CreateMessage("My new car is great! ;)");
    observer5->RemoveMeFromList();

    observer4->RemoveMeFromList();
    observer1->RemoveMeFromList();

    delete observer1;
    delete observer2;
    delete observer3;
    delete observer4;
    delete observer5;
    delete subject;
    return 0;
}