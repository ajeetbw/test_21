package com.example.mockrepo;

import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

/**
 * A complete, self-contained Enterprise Task Management System.
 * Demonstrates OOP principles, concurrency structures, and advanced Java features.
 */
public class TaskManagementSystem {

    // --- Data Models ---

    public enum TaskStatus {
        TODO, IN_PROGRESS, REVIEW, COMPLETED
    }

    public enum TaskPriority {
        LOW, MEDIUM, HIGH, CRITICAL
    }

    public static class Task {
        private final int id;
        private String title;
        private String description;
        private TaskStatus status;
        private TaskPriority priority;
        private final LocalDateTime createdAt;
        private LocalDateTime updatedAt;

        public Task(int id, String title, String description, TaskPriority priority) {
            this.id = id;
            this.title = title;
            this.description = description;
            this.status = TaskStatus.TODO;
            this.priority = priority;
            this.createdAt = LocalDateTime.now();
            this.updatedAt = this.createdAt;
        }

        // Getters and Setters
        public int getId() { return id; }
        public String getTitle() { return title; }
        public TaskStatus getStatus() { return status; }
        public TaskPriority getPriority() { return priority; }

        public void setStatus(TaskStatus status) {
            this.status = status;
            this.updatedAt = LocalDateTime.now();
        }

        @Override
        public String toString() {
            DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm");
            return String.format("[%d] %s (%s) - Priority: %s | Created: %s", 
                id, title, status, priority, createdAt.format(formatter));
        }
    }

    // --- Exceptions ---

    public static class TaskNotFoundException extends Exception {
        public TaskNotFoundException(String message) {
            super(message);
        }
    }

    // --- Services ---

    public static class TaskManager {
        private final Map<Integer, Task> database = new ConcurrentHashMap<>();
        private final AtomicInteger idGenerator = new AtomicInteger(1);

        /**
         * Creates a new task and stores it in the database.
         */
        public Task createTask(String title, String description, TaskPriority priority) {
            if (title == null || title.trim().isEmpty()) {
                throw new IllegalArgumentException("Title cannot be empty");
            }
            int newId = idGenerator.getAndIncrement();
            Task newTask = new Task(newId, title, description, priority);
            database.put(newId, newTask);
            System.out.println("Created: " + newTask.getTitle());
            return newTask;
        }

        /**
         * Updates the status of an existing task.
         */
        public void updateTaskStatus(int id, TaskStatus newStatus) throws TaskNotFoundException {
            Task task = database.get(id);
            if (task == null) {
                throw new TaskNotFoundException("Task with ID " + id + " does not exist.");
            }
            task.setStatus(newStatus);
            System.out.println("Updated Task " + id + " status to " + newStatus);
        }

        /**
         * Retrieves all tasks sorted by priority (CRITICAL first).
         */
        public List<Task> getAllTasksSortedByPriority() {
            List<Task> tasks = new ArrayList<>(database.values());
            tasks.sort((t1, t2) -> t2.getPriority().compareTo(t1.getPriority()));
            return tasks;
        }

        /**
         * Generates a summary report of tasks grouped by status.
         */
        public void printStatusReport() {
            Map<TaskStatus, Integer> counts = new EnumMap<>(TaskStatus.class);
            for (TaskStatus status : TaskStatus.values()) {
                counts.put(status, 0);
            }
            
            for (Task task : database.values()) {
                counts.put(task.getStatus(), counts.get(task.getStatus()) + 1);
            }

            System.out.println("\n--- Task Status Report ---");
            counts.forEach((status, count) -> 
                System.out.println(String.format("%-15s : %d", status, count))
            );
            System.out.println("--------------------------\n");
        }
    }

    // --- Main Execution ---

    public static void main(String[] args) {
        System.out.println("Initializing Task Management System...");
        TaskManager manager = new TaskManager();

        try {
            manager.createTask("Setup SVN Repository", "Initialize the base SVN server and setup hooks.", TaskPriority.CRITICAL);
            manager.createTask("Develop Git Migration Agent", "Write the Python script that runs git-svn.", TaskPriority.HIGH);
            manager.createTask("Write Unit Tests", "Achieve 80% code coverage on the agent logic.", TaskPriority.MEDIUM);
            manager.createTask("Deploy to Staging", "Push the new agent container to the staging environment.", TaskPriority.HIGH);

            System.out.println("\n--- Initial Task List ---");
            for (Task task : manager.getAllTasksSortedByPriority()) {
                System.out.println(task);
            }

            manager.updateTaskStatus(1, TaskStatus.COMPLETED);
            manager.updateTaskStatus(2, TaskStatus.IN_PROGRESS);

            manager.printStatusReport();
            System.out.println("System shutdown graceful.");

        } catch (Exception e) {
            System.err.println("Critical System Failure: " + e.getMessage());
            e.printStackTrace();
        }
    }
}