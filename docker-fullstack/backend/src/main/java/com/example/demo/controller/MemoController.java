package com.example.demo.controller;

import com.example.demo.entity.Memo;
import com.example.demo.repository.MemoRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Optional;

@RestController
@RequestMapping("/api/memos")
public class MemoController {

    @Autowired
    private MemoRepository memoRepository;

    @GetMapping
    public ResponseEntity<List<Memo>> getAllMemos() {
        List<Memo> memos = memoRepository.findAllByOrderByCreatedAtDesc();
        return ResponseEntity.ok(memos);
    }

    @GetMapping("/{id}")
    public ResponseEntity<Memo> getMemoById(@PathVariable Long id) {
        Optional<Memo> memo = memoRepository.findById(id);
        return memo.map(ResponseEntity::ok)
                .orElse(ResponseEntity.notFound().build());
    }

    @PostMapping
    public ResponseEntity<Memo> createMemo(@RequestBody Memo memo) {
        if (memo.getTitle() == null || memo.getTitle().trim().isEmpty()) {
            return ResponseEntity.badRequest().build();
        }
        Memo savedMemo = memoRepository.save(memo);
        return ResponseEntity.status(HttpStatus.CREATED).body(savedMemo);
    }

    @PutMapping("/{id}")
    public ResponseEntity<Memo> updateMemo(@PathVariable Long id, @RequestBody Memo memoDetails) {
        Optional<Memo> optionalMemo = memoRepository.findById(id);
        if (optionalMemo.isEmpty()) {
            return ResponseEntity.notFound().build();
        }

        Memo memo = optionalMemo.get();
        if (memoDetails.getTitle() != null) {
            memo.setTitle(memoDetails.getTitle());
        }
        if (memoDetails.getContent() != null) {
            memo.setContent(memoDetails.getContent());
        }

        Memo updatedMemo = memoRepository.save(memo);
        return ResponseEntity.ok(updatedMemo);
    }

    @DeleteMapping("/{id}")
    public ResponseEntity<Void> deleteMemo(@PathVariable Long id) {
        if (!memoRepository.existsById(id)) {
            return ResponseEntity.notFound().build();
        }
        memoRepository.deleteById(id);
        return ResponseEntity.noContent().build();
    }
}

