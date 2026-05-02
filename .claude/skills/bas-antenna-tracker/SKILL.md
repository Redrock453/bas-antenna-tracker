```markdown
# bas-antenna-tracker Development Patterns

> Auto-generated skill from repository analysis

## Overview
This skill teaches you the core development patterns and conventions used in the `bas-antenna-tracker` TypeScript repository. You'll learn about file naming, import/export styles, how to write and organize tests, and the typical workflows for contributing to the codebase. This guide is ideal for onboarding new contributors or maintaining consistency across the project.

## Coding Conventions

### File Naming
- Use **camelCase** for file names.
  - Example: `antennaTracker.ts`, `signalProcessor.ts`

### Import Style
- Use **relative imports** for referencing modules.
  - Example:
    ```typescript
    import { calculateAngle } from './angleUtils';
    ```

### Export Style
- Use **named exports** for all modules.
  - Example:
    ```typescript
    // angleUtils.ts
    export function calculateAngle(a: number, b: number): number {
      return a - b;
    }
    ```

### Commit Messages
- Freeform style, sometimes with prefixes.
- Average length: ~49 characters.
  - Example: `Add signal strength calculation function`

## Workflows

### Adding a New Feature
**Trigger:** When implementing a new capability or module  
**Command:** `/add-feature`

1. Create a new file using camelCase naming.
2. Write your TypeScript code, using named exports.
3. Use relative imports to reference other modules.
4. Add or update corresponding test files (`*.test.ts`).
5. Commit your changes with a clear, concise message.

### Fixing a Bug
**Trigger:** When resolving a defect or issue  
**Command:** `/fix-bug`

1. Locate the relevant file(s) and make necessary corrections.
2. Update or add tests to cover the bug fix.
3. Run all tests to ensure nothing is broken.
4. Commit with a message describing the fix.

### Writing and Running Tests
**Trigger:** When validating code correctness  
**Command:** `/run-tests`

1. Create or update test files matching the `*.test.ts` pattern.
2. Use the project's (undetected) test framework to run tests.
3. Ensure all tests pass before committing.

## Testing Patterns

- Test files follow the `*.test.ts` naming convention.
- The specific test framework is not detected, but standard TypeScript test syntax applies.
- Example test file:
  ```typescript
  // antennaTracker.test.ts
  import { calculateAngle } from './angleUtils';

  test('calculateAngle returns correct difference', () => {
    expect(calculateAngle(90, 30)).toBe(60);
  });
  ```

## Commands
| Command      | Purpose                                 |
|--------------|-----------------------------------------|
| /add-feature | Start the workflow for adding a feature |
| /fix-bug     | Start the workflow for fixing a bug     |
| /run-tests   | Run all tests in the codebase           |
```
